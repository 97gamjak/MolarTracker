#include "account_controller.hpp"

#include <qstackedwidget.h>

#include <QAction>
#include <QMainWindow>
#include <QObject>
#include <format>

#include "cache/account_cache.hpp"
#include "commands/undo_stack.hpp"
#include "controller/helpers.hpp"
#include "controller/mapper/account_mapper.hpp"
#include "drafts/position_draft.hpp"
#include "finance/price_cache.hpp"
#include "helpers.hpp"
#include "logging/log_macros.hpp"
#include "side_bar/account_controller.hpp"
#include "ui/account/account_detail_view.hpp"
#include "ui/utils/error.hpp"

REGISTER_LOG_CATEGORY("Controller.AccountSideBarController");

namespace controller
{
    /**
     * @brief Detail struct for AccountController, containing references to
     * stores, the undo stack, and UI components.
     *
     */
    struct AccountController::Details
    {
        /// Reference to the undo stack
        cmd::UndoStack& undoStack;

        /// Reference to the price cache
        std::shared_ptr<finance::PriceCache> priceCache;

        /// Reference to the account store
        std::shared_ptr<cache::AccountCache> accountCache;
        /// Reference to the position store
        std::shared_ptr<store::IPositionStore> positionStore;
        /// Reference to the transaction store
        std::shared_ptr<store::ITransactionStore> transactionStore;
        /// Reference to the stock cache
        std::shared_ptr<cache::StockCache> stockCache;

        /// Pointer to the stacked widget
        QStackedWidget* stackedWidget;
        /// Pointer to the account detail view
        QPointer<ui::AccountDetailView> accountDetailView;

        /// Pointer to the connections object for managing signal-slot
        /// connections
        std::unique_ptr<Connections> connections;

        /// The currently selected account, stored as an optional AccountDraft
        /// for display purposes
        std::unique_ptr<drafts::AccountDraft> currentAccount;

        /// A mapping of account IDs to their corresponding open stock position
        /// details, used for displaying the account details in the UI
        IdMap<AccountId, std::vector<OpenStockPositionDetail>>
            openPositionDetails;

        Details(
            const std::shared_ptr<cache::AccountCache>&      accountCache_,
            const std::shared_ptr<store::IPositionStore>&    positionStore_,
            const std::shared_ptr<cache::StockCache>&        stockCache_,
            const std::shared_ptr<store::ITransactionStore>& transactionStore_,
            const std::shared_ptr<finance::PriceCache>&      priceCache_,
            cmd::UndoStack&                                  undoStack_,
            QStackedWidget*                                  stackedWidget_
        );
        ~Details() = default;

        // delete copy and move constructor and assignment operator
        Details(const Details&)            = delete;
        Details& operator=(const Details&) = delete;
        Details(Details&&)                 = delete;
        Details& operator=(Details&&)      = delete;
    };

    /**
     * @brief Construct a new Account Controller:: Details:: Details object
     *
     * @param accountCache_
     * @param positionStore_
     * @param stockCache_
     * @param transactionStore_
     * @param priceCache_
     * @param undoStack_
     * @param stackedWidget_
     */
    AccountController::Details::Details(
        const std::shared_ptr<cache::AccountCache>&      accountCache_,
        const std::shared_ptr<store::IPositionStore>&    positionStore_,
        const std::shared_ptr<cache::StockCache>&        stockCache_,
        const std::shared_ptr<store::ITransactionStore>& transactionStore_,
        const std::shared_ptr<finance::PriceCache>&      priceCache_,
        cmd::UndoStack&                                  undoStack_,
        QStackedWidget*                                  stackedWidget_
    )
        : undoStack(undoStack_),
          priceCache(priceCache_),
          accountCache(accountCache_),
          positionStore(positionStore_),
          transactionStore(transactionStore_),
          stockCache(stockCache_),
          stackedWidget(stackedWidget_),
          accountDetailView(new ui::AccountDetailView(stackedWidget)),
          connections(std::make_unique<Connections>())

    {
    }

    /**
     * @brief Controller for managing account-related actions
     *
     * @param undoStack
     * @param accountCache
     * @param positionStore
     * @param stockCache
     * @param transactionStore
     * @param priceCache
     * @param stackedWidget
     */
    AccountController::AccountController(
        cmd::UndoStack&                                  undoStack,
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<store::ITransactionStore>& transactionStore,
        const std::shared_ptr<cache::AccountCache>&      accountCache,
        const std::shared_ptr<cache::StockCache>&        stockCache,
        const std::shared_ptr<finance::PriceCache>&      priceCache,
        QStackedWidget*                                  stackedWidget
    )
        : _details(
              std::make_unique<Details>(
                  accountCache,
                  positionStore,
                  stockCache,
                  transactionStore,
                  priceCache,
                  undoStack,
                  stackedWidget
              )
          )
    {
        _details->stackedWidget->addWidget(_details->accountDetailView);

        _details->connections->add(_details->priceCache->subscribeToPriceChange(
            [this]()
            {
                if (!_details->accountDetailView ||
                    _details->currentAccount == nullptr)
                    return;

                auto& details =
                    _details->openPositionDetails[_details->currentAccount
                                                      ->getId()];
                std::vector<drafts::PositionStockDetailDraft> drafts;
                for (auto& detail : details)
                {
                    const auto quote = _details->priceCache->get(detail.ticker);
                    if (quote.has_value())
                    {
                        detail.pnl->setCurrentPrice(quote.value().getPrice());
                        detail.positionDraft.updateUnrealizedPnL(
                            quote.value().getPrice(),
                            detail.pnl->getMarketValue(),
                            detail.pnl->getUnrealizedPnL(),
                            detail.pnl->getUnrealizedPnLPercentage()
                        );
                    }
                    drafts.push_back(detail.positionDraft);
                }

                _details->accountDetailView->updateSecurityAccount(
                    *_details->currentAccount,
                    drafts
                );
            },
            this
        ));
    }

    AccountController::~AccountController() = default;

    /**
     * @brief Handle the selection of an account in the side bar
     *
     * @param id The ID of the selected account
     */
    void AccountController::accountSelected(AccountId id)
    {
        LOG_ENTRY;

        const auto accountResult =
            cache::AccountCacheUtils::getAccount(id, _details->accountCache);

        if (!accountResult)
        {
            ui::ErrorDialog::show(
                accountResult.error(),
                "Account with ID " + id.toString() + " not found"
            );
            return;
        }

        const auto& account = *accountResult;

        const auto accountDraft = AccountMapper::toDraft(*account);

        switch (account->getKind())
        {
            case AccountKind::Cash:
                _details->accountDetailView->updateCashAccount(accountDraft);
                break;
            case AccountKind::Security:
            {
                _details->openPositionDetails[account->getId()] =
                    getOpenStockPositionDetails(
                        account->getId(),
                        _details->positionStore,
                        _details->stockCache,
                        _details->accountCache,
                        _details->transactionStore
                    );

                LOG_DEBUG(
                    std::format(
                        "Retrieved {} open position drafts for account {}",
                        _details->openPositionDetails[account->getId()].size(),
                        account->getName()
                    )
                );

                std::vector<drafts::PositionStockDetailDraft> details;
                for (const auto& detail :
                     _details->openPositionDetails[account->getId()])
                {
                    details.push_back(detail.positionDraft);
                }

                _details->accountDetailView->updateSecurityAccount(
                    accountDraft,
                    details
                );
                _details->currentAccount =
                    std::make_unique<drafts::AccountDraft>(accountDraft);
                break;
            }
            case AccountKind::External:
            {
                LOG_ERROR("Cannot display external accounts");
                return;
            }
        }

        _details->stackedWidget->setCurrentWidget(_details->accountDetailView);
    }

}   // namespace controller
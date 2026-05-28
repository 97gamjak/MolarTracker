#include "account_controller.hpp"

#include <qstackedwidget.h>

#include <QAction>
#include <QMainWindow>
#include <QObject>
#include <format>

#include "commands/account/create_account_command.hpp"
#include "commands/undo_stack.hpp"
#include "controller/helpers.hpp"
#include "drafts/account_mapper.hpp"
#include "drafts/position_draft.hpp"
#include "helpers.hpp"
#include "logging/log_macros.hpp"
#include "side_bar/account_controller.hpp"
#include "store/i_account_store.hpp"
#include "ui/account/account_detail_view.hpp"

REGISTER_LOG_CATEGORY("UI.Controller.AccountSideBarController");

namespace controller
{
    /**
     * @brief Stores for managing account-related data
     *
     */
    struct AccountController::Stores
    {
        /// Reference to the account store
        std::shared_ptr<store::IAccountStore> accountStore;
        /// Reference to the position store
        store::PositionStore& positionStore;
        /// Reference to the stock store
        std::shared_ptr<store::IStockStore> stockStore;
        /// Reference to the transaction store
        std::shared_ptr<store::ITransactionStore> transactionStore;

        Stores(
            const std::shared_ptr<store::IAccountStore>&     accountStore_,
            store::PositionStore&                            positionStore_,
            const std::shared_ptr<store::IStockStore>&       stockStore_,
            const std::shared_ptr<store::ITransactionStore>& transactionStore_
        );
        ~Stores() = default;

        // delete copy and move constructor and assignment operator
        Stores(const Stores&)            = delete;
        Stores& operator=(const Stores&) = delete;
        Stores(Stores&&)                 = delete;
        Stores& operator=(Stores&&)      = delete;
    };

    /**
     * @brief Construct a new Account Controller:: Stores:: Stores object
     *
     * @param accountStore_
     * @param positionStore_
     * @param stockStore_
     * @param transactionStore_
     */
    AccountController::Stores::Stores(
        const std::shared_ptr<store::IAccountStore>&     accountStore_,
        store::PositionStore&                            positionStore_,
        const std::shared_ptr<store::IStockStore>&       stockStore_,
        const std::shared_ptr<store::ITransactionStore>& transactionStore_
    )
        : accountStore(accountStore_),
          positionStore(positionStore_),
          stockStore(stockStore_),
          transactionStore(transactionStore_)
    {
    }

    /**
     * @brief Controller for managing account-related actions
     *
     * @param undoStack
     * @param accountStore
     * @param positionStore
     * @param stockStore
     * @param transactionStore
     * @param stackedWidget
     */
    AccountController::AccountController(
        cmd::UndoStack&                                  undoStack,
        const std::shared_ptr<store::IAccountStore>&     accountStore,
        store::PositionStore&                            positionStore,
        const std::shared_ptr<store::IStockStore>&       stockStore,
        const std::shared_ptr<store::ITransactionStore>& transactionStore,
        QStackedWidget*                                  stackedWidget
    )
        : _undoStack(undoStack),
          _stores(
              std::make_unique<Stores>(
                  accountStore,
                  positionStore,
                  stockStore,
                  transactionStore
              )
          ),
          _stackedWidget(stackedWidget),
          _accountDetailView(new ui::AccountDetailView(_stackedWidget))
    {
        _stackedWidget->addWidget(_accountDetailView);
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

        const auto account = _stores->accountStore->getAccount(id);

        if (!account.has_value())
        {
            LOG_WARNING(
                std::format("Account with ID {} not found", id.value())
            );
            return;
        }

        switch (account->getKind())
        {
            case AccountKind::Cash:
                _accountDetailView->updateCashAccount(
                    drafts::AccountMapper::toDraft(account.value())
                );
                break;
            case AccountKind::Security:
            {
                const std::vector<drafts::PositionDetailDraft> drafts =
                    getOpenPositionDrafts(
                        account->getId(),
                        _stores->positionStore,
                        _stores->stockStore,
                        _stores->transactionStore
                    );

                LOG_DEBUG(
                    std::format(
                        "Retrieved {} open position drafts for account {}",
                        drafts.size(),
                        account->getName()
                    )
                );

                _accountDetailView->updateSecurityAccount(
                    drafts::AccountMapper::toDraft(account.value()),
                    drafts
                );
                break;
            }
            case AccountKind::External:
            {
                LOG_ERROR("Cannot display external accounts");
                return;
            }
        }

        _stackedWidget->setCurrentWidget(_accountDetailView);
    }

}   // namespace controller
#include "account_controller.hpp"

#include <qstackedwidget.h>

#include <QAction>
#include <QMainWindow>
#include <QObject>
#include <format>

#include "app/store/account/account_store.hpp"
#include "commands/account/create_account_command.hpp"
#include "commands/undo_stack.hpp"
#include "controller/helpers.hpp"
#include "drafts/position_draft.hpp"
#include "helpers.hpp"
#include "logging/log_macros.hpp"
#include "side_bar/account_controller.hpp"
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
        app::AccountStore& accountStore;
        /// Reference to the position store
        app::PositionStore& positionStore;
        /// Reference to the stock store
        app::StockStore& stockStore;
        /// Reference to the transaction store
        app::TransactionStore& transactionStore;

        Stores(
            app::AccountStore&     accountStore_,
            app::PositionStore&    positionStore_,
            app::StockStore&       stockStore_,
            app::TransactionStore& transactionStore_
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
        app::AccountStore&     accountStore_,
        app::PositionStore&    positionStore_,
        app::StockStore&       stockStore_,
        app::TransactionStore& transactionStore_
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
        cmd::UndoStack&        undoStack,
        app::AccountStore&     accountStore,
        app::PositionStore&    positionStore,
        app::StockStore&       stockStore,
        app::TransactionStore& transactionStore,
        QStackedWidget*        stackedWidget
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

        const auto account = _stores->accountStore.getAccount(id);

        if (!account.has_value())
        {
            LOG_WARNING(
                std::format("Account with ID {} not found", id.value())
            );
            return;
        }

        switch (account->kind)
        {
            case AccountKind::Cash:
                _accountDetailView->updateCashAccount(account.value());
                break;
            case AccountKind::Security:
            {
                const std::vector<drafts::PositionDetailDraft> drafts =
                    getOpenPositionDrafts(
                        account->id,
                        _stores->positionStore,
                        _stores->stockStore,
                        _stores->transactionStore
                    );

                LOG_DEBUG(
                    std::format(
                        "Retrieved {} open position drafts for account {}",
                        drafts.size(),
                        account->name
                    )
                );

                _accountDetailView->updateSecurityAccount(
                    account.value(),
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
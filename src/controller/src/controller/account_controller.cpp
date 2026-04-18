#include "account_controller.hpp"

#include <qstackedwidget.h>

#include <QAction>
#include <QMainWindow>
#include <QObject>

#include "app/store/account_store.hpp"
#include "commands/account/create_account_command.hpp"
#include "commands/undo_stack.hpp"
#include "logging/log_macros.hpp"
#include "side_bar/account_controller.hpp"
#include "ui/account/account_detail_view.hpp"

REGISTER_LOG_CATEGORY("UI.Controller.AccountSideBarController");

namespace controller
{
    /**
     * @brief Controller for managing account-related actions
     *
     * @param undoStack
     * @param accountStore
     * @param stackedWidget
     */
    AccountController::AccountController(
        cmd::UndoStack&    undoStack,
        app::AccountStore& accountStore,
        QStackedWidget*    stackedWidget
    )
        : _undoStack(undoStack),
          _accountStore(accountStore),
          _stackedWidget(stackedWidget),
          _accountDetailView(new ui::AccountDetailView(_stackedWidget))
    {
        _stackedWidget->addWidget(_accountDetailView);
    }

    /**
     * @brief Handle the selection of an account in the side bar
     *
     * @param id The ID of the selected account
     */
    void AccountController::accountSelected(AccountId id)
    {
        LOG_ENTRY;

        const auto account = _accountStore.getAccount(id);

        if (!account.has_value())
        {
            LOG_WARNING(
                std::format("Account with ID {} not found", id.value())
            );
            return;
        }

        _accountDetailView->updateAccount(account.value());
        _stackedWidget->setCurrentWidget(_accountDetailView);
    }

}   // namespace controller
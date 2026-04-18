#include "account_controller.hpp"

#include <qstackedwidget.h>

#include <QAction>
#include <QMainWindow>
#include <QObject>

#include "app/app_context.hpp"
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
     * @param undoStack A reference to the undo stack, this is used to push
     * commands that are created as a result of actions in the account category
     * (e.g. creating a new account), this allows the user to undo and redo
     * actions related to accounts using the undo stack.
     * @param appContext A reference to the application context, this is used to
     * access the stores and services needed to perform operations related to
     * accounts (e.g. creating a new account), this allows the controller to
     * interact with the underlying data and business logic for accounts, and
     * ensures that the controller can perform the necessary operations to
     * manage accounts effectively.
     * @param stackedWidget A pointer to the stacked widget, this is used to
     * manage the different views for account details and other related
     * information, allowing the controller to switch between views as needed.
     */
    AccountController::AccountController(
        cmd::UndoStack&  undoStack,
        app::AppContext& appContext,
        QStackedWidget*  stackedWidget
    )
        : _undoStack(undoStack),
          _appContext(appContext),
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

        const auto account =
            _appContext.getStore().getAccountStore().getAccount(id);

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
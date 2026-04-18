#include "account_controller.hpp"

#include <QMainWindow>

#include "app/app_context.hpp"
#include "commands/account/create_account_command.hpp"
#include "commands/undo_stack.hpp"
#include "logging/log_macros.hpp"
#include "ui/account/create_account_dlg.hpp"
#include "ui/side_bar/account_category.hpp"
#include "ui/side_bar/account_item.hpp"
#include "utils/qt_helpers.hpp"

REGISTER_LOG_CATEGORY("Controller.SideBar.AccountSideBarController");

namespace controller
{
    /**
     * @brief Construct a new Account Side Bar Controller:: Account Side Bar
     * Controller object
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
     * @param mainWindow A pointer to the main window, this is used as the
     * parent widget for dialogs that are opened as a result of actions in the
     * account category (e.g. the create account dialog), this ensures that the
     * dialogs are properly parented in the UI and will be modal to the main
     * window when opened.
     *
     */
    AccountSideBarController::AccountSideBarController(
        cmd::UndoStack&    undoStack,
        app::AppContext&   appContext,
        AccountController& accountController,
        QMainWindow*       mainWindow
    )
        : SideBarCategoryController(new ui::AccountCategory(), mainWindow),
          _undoStack(undoStack),
          _appContext(appContext),
          _accountController(accountController)
    {
    }

    /**
     * @brief Refresh the account category in the side bar, this will clear all
     * existing accounts from the category and add the current accounts to the
     * category, this should be called whenever the accounts data changes to
     * ensure the side bar is up to date
     *
     */
    void AccountSideBarController::refresh()
    {
        auto* category = dynamic_cast<ui::AccountCategory*>(getCategory());
        if (category == nullptr)
            return;

        category->clearAccounts();
        const auto accounts =
            _appContext.getStore().getAccountStore().getAllAccounts();

        for (const auto& account : accounts)
        {
            category->addAccount(
                account.id,
                QString::fromStdString(account.name)
            );
        }
    }

    /**
     * @brief Handle a context menu action triggered for the account category,
     * this will be called when an action in the context menu of the account
     * category is triggered, and should handle the action accordingly (e.g. if
     * the create account action is triggered, it should open a dialog to create
     * a new account)
     *
     * @param item The account category item for which the context menu action
     * was triggered
     * @param action The action that was triggered
     */
    void AccountSideBarController::handleContextMenuAction(
        const ui::AccountCategory* item,
        const QAction*             action
    )
    {
        if (item == nullptr || action == nullptr)
        {
            LOG_WARNING(
                "Context menu action triggered with null item or action, "
                "ignoring"
            );
            return;
        }

        if (action == item->getCreateAction())
        {
            LOG_DEBUG("Create Account action triggered");

            _createAccountDialog =
                utils::makeQChild<ui::CreateAccountDialog>(getMainWindow());

            connect(
                _createAccountDialog,
                &ui::CreateAccountDialog::requested,
                this,
                &AccountSideBarController::_onCreateAccountRequested
            );

            if (auto* dialog = _createAccountDialog.data())
                dialog->exec();
        }
    }

    /**
     * @brief Handle the creation of a new account based on the given account
     * draft, this will be called when the create account dialog emits the
     * requested signal with the account draft, and should handle creating a new
     * account based on the information in the draft (e.g. by calling a method
     * on the store to add a new account), this allows the controller to take
     * the user input from the dialog and use it to create a new account in the
     * underlying data, and ensures that the creation of new accounts is handled
     * in a consistent way through the controller.
     *
     * @param account The account draft containing the information needed to
     * create a new account, this includes details such as the name, currency,
     * and kind of account to create, and should be used by the controller to
     * create a new account in the underlying data.
     */
    void AccountSideBarController::_onCreateAccountRequested(
        const drafts::AccountDraft& account
    )
    {
        LOG_INFO("Create Account requested with name: " + account.name);

        if (account.kind == AccountKind::Cash)
        {
            cmd::Commands command("Create Account");

            auto result = cmd::Commands::makeAndDo<cmd::CreateAccountCommand>(
                _appContext.getStore().getAccountStore(),
                account
            );

            if (!result)
            {
                const auto& error = result.error();
                const auto  msg =
                    "Failed to create account: " + error->getMessage();
                LOG_ERROR(msg);
                return;
            }

            command << std::move(result);

            _undoStack.push(std::move(command));
        }
        else
        {
            LOG_WARNING(
                "Unsupported account kind: " +
                AccountKindMeta::toString(account.kind)
            );
        }

        refresh();
    }

    /**
     * @brief Handle the selection of an account in the side bar
     *
     * @param id The ID of the selected account
     */
    void AccountSideBarController::onAccountSelected(AccountId id)
    {
        LOG_DEBUG(std::format("Account with ID {} selected", id.value()));

        _accountController.accountSelected(id);
    }
}   // namespace controller
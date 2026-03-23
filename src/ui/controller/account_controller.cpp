#include "account_controller.hpp"

#include <QAction>
#include <QMainWindow>
#include <QObject>

#include "app/app_context.hpp"
#include "drafts/account_draft.hpp"
#include "logging/log_macros.hpp"
#include "ui/commands/account/create_account_command.hpp"
#include "ui/commands/undo_stack.hpp"
#include "ui/widgets/account/create_account_dlg.hpp"
#include "ui/widgets/side_bar/account_category.hpp"

REGISTER_LOG_CATEGORY("UI.Controller.AccountSideBarController");

namespace ui
{
    /**
     * @brief Construct a new Account Side Bar Controller:: Account Side Bar
     * Controller object
     *
     */
    AccountSideBarController::AccountSideBarController(
        UndoStack&       undoStack,
        app::AppContext& appContext,
        QMainWindow*     mainWindow
    )
        : SideBarCategoryController(new AccountCategory(), mainWindow),
          _undoStack(undoStack),
          _appContext(appContext)
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
        auto* category = dynamic_cast<AccountCategory*>(getCategory());
        if (category == nullptr)
            return;

        category->clearAccounts();
        const auto accounts =
            _appContext.getStore().getAccountStore().getAllAccounts();

        for (const auto* account : accounts)
        {
            category->addAccount(
                account->getId(),
                QString::fromStdString(account->getName())
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
        const AccountCategory* item,
        const QAction*         action
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
            LOG_INFO("Create Account action triggered");

            // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
            _createAccountDialog = new CreateAccountDialog(getMainWindow());

            connect(
                _createAccountDialog,
                &CreateAccountDialog::requested,
                this,
                &AccountSideBarController::_onCreateAccountRequested
            );

            _createAccountDialog->exec();
        }
    }

    void AccountSideBarController::_onCreateAccountRequested(
        drafts::AccountDraft account
    )
    {
        LOG_INFO("Create Account requested with name: " + account.name);

        if (account.kind == AccountKind::Cash)
        {
            Commands command("Create Account");

            auto result = Commands::makeAndDo<CreateAccountCommand>(
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

}   // namespace ui
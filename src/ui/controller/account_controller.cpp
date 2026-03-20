#include "account_controller.hpp"

#include "logging/log_macros.hpp"
#include "ui/widgets/side_bar/account_category.hpp"

REGISTER_LOG_CATEGORY("UI.Controller.AccountSideBarController");

namespace ui
{
    /**
     * @brief Construct a new Account Side Bar Controller:: Account Side Bar
     * Controller object
     *
     */
    AccountSideBarController::AccountSideBarController()
        : SideBarCategoryController(new AccountCategory())
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
        category->addAccount(1, "Account 1");
        category->addAccount(2, "Account 2");
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
            // Handle create account action
        }
    }

}   // namespace ui
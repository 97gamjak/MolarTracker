#include "account_category.hpp"

#include <QAction>
#include <QMenu>
#include <QString>

#include "account_item.hpp"

namespace ui
{
    /**
     * @brief Construct a new Account Category:: Account Category object
     *
     */
    AccountCategory::AccountCategory() : Category("Accounts") {}

    /**
     * @brief Add an account to the category, this will create a new account
     * item and add it to the category in the side bar
     *
     * @param id The id of the account, used to identify which account is
     * selected when the itemSelected signal is emitted
     * @param name The name of the account to display in the side bar
     */
    void AccountCategory::addAccount(int id, const QString& name)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* accountItem = new AccountItem(id, name);
        appendRow(accountItem);
    }

    /**
     * @brief Clear all accounts from the category, this will remove all account
     * items from the category in the side bar
     *
     */
    void AccountCategory::clearAccounts() { removeRows(0, rowCount()); }

    /**
     * @brief Populate the context menu of the account category, this will be
     * called when the account category is right-clicked, and the menu will be
     * shown to the user
     *
     * @param menu The menu to populate, the account category should add its
     * actions to this menu
     */
    void AccountCategory::populateContextMenu(QMenu& menu)
    {
        _createAction = menu.addAction("Create Account");
    }

}   // namespace ui
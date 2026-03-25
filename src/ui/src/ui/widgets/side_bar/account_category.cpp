#include "ui/side_bar/account_category.hpp"

#include <QAction>
#include <QMenu>
#include <QString>

#include "ui/side_bar/account_item.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{
    /**
     * @brief Construct a new Account Category:: Account Category object
     *
     */
    AccountCategory::AccountCategory()
        : Category("Accounts", SideBarItemType::AccountCategory),
          _createAction(nullptr)
    {
    }

    /**
     * @brief Add an account to the category, this will create a new account
     * item and add it to the category in the side bar
     *
     * @param id The id of the account, used to identify which account is
     * selected when the itemSelected signal is emitted
     * @param name The name of the account to display in the side bar
     */
    void AccountCategory::addAccount(AccountId id, const QString& name)
    {
        auto* accountItem = utils::makeQChild<AccountItem>(id, name);
        appendRow(accountItem);
    }

    /**
     * @brief Clear all accounts from the category, this will remove all account
     * items from the category in the side bar
     *
     */
    void AccountCategory::clearAccounts() { removeRows(0, rowCount()); }

    /**
     * @brief Get the Create Action object, this is used to connect the action
     * to a slot in the controller that will handle creating a new account when
     * the action is triggered
     *
     * @return QAction*
     */
    QAction* AccountCategory::getCreateAction() const { return _createAction; }

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
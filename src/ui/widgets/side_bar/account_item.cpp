#include "account_item.hpp"

#include <QAction>
#include <QMenu>
#include <QString>

namespace ui
{
    /**
     * @brief Construct a new Account Item:: Account Item object
     *
     * @param id The id of the account, used to identify which account is
     * selected when the itemSelected signal is emitted
     * @param name The name of the account to display in the side bar
     */
    AccountItem::AccountItem(int id, const QString& name)
        : SideBarItem(name, SideBarItemType::Accounts), _id(id)
    {
    }

    /**
     * @brief Populate the context menu of the account item, this will be called
     * when the account item is right-clicked, and the menu will be shown to the
     * user
     *
     * @param menu The menu to populate, the account item should add its actions
     * to this menu
     */
    void AccountItem::populateContextMenu(QMenu& menu)
    {
        _openAction   = menu.addAction("Open");
        _deleteAction = menu.addAction("Delete");
    }

}   // namespace ui
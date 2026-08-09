#include "ui/side_bar/account_item.hpp"

#include <QAction>
#include <QMenu>
#include <QString>

#include "common/finance.hpp"

namespace ui
{
    namespace
    {
        /**
         * @brief Get the icon for the account item based on the account kind
         *
         * @param kind The kind of the account
         * @return QIcon The icon for the account item
         */
        [[nodiscard]]
        QIcon getAccountIcon(AccountKind kind)
        {
            switch (kind)
            {
                case AccountKind::Cash:
                    return QIcon(":/icons/account_cash.svg");
                case AccountKind::Security:
                    return QIcon(":/icons/account_security.svg");
                case AccountKind::External:
                    return QIcon("");
            }

            std::unreachable();
        }
    }   // namespace
    /**
     * @brief Construct a new Account Item:: Account Item object
     *
     * @param id The id of the account, used to identify which account is
     * selected when the itemSelected signal is emitted
     * @param name The name of the account to display in the side bar
     */
    AccountItem::AccountItem(
        AccountId      id,
        AccountKind    kind,
        const QString& name
    )
        : SideBarItem(SideBarItemType::AccountsItem),
          _accountId(id),
          _openAction(nullptr),
          _deleteAction(nullptr)
    {
        setFlags(flags() | Qt::ItemIsEditable);
        setText(name);
        setIcon(getAccountIcon(kind));
    }

    /**
     * @brief Get the id of the account, this is used to
     * identify which account is selected when the itemSelected
     * signal is emitted
     *
     * @return AccountId The id of the account
     */
    AccountId AccountItem::getId() const { return _accountId; }

    /**
     * @brief Get the open action of the account item, this is
     * used to connect the open action to the respective
     * functionality when the context menu is populated
     *
     * @return QAction* The open action of the account item
     */
    QAction* AccountItem::getOpenAction() const { return _openAction; }

    /**
     * @brief Get the delete action of the account item, this is
     * used to connect the delete action to the respective
     * functionality when the context menu is populated
     *
     * @return QAction* The delete action of the account item
     */
    QAction* AccountItem::getDeleteAction() const { return _deleteAction; }

    /**
     * @brief Populate the context menu of the account item,
     * this will be called when the account item is
     * right-clicked, and the menu will be shown to the user
     *
     * @param menu The menu to populate, the account item should
     * add its actions to this menu
     */
    void AccountItem::populateContextMenu(QMenu& menu)
    {
        _openAction   = menu.addAction("Open");
        _deleteAction = menu.addAction("Delete");
    }

}   // namespace ui

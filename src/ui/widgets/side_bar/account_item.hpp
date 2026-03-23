#ifndef __UI__WIDGETS__SIDE_BAR__ACCOUNT_ITEM_HPP__
#define __UI__WIDGETS__SIDE_BAR__ACCOUNT_ITEM_HPP__

#include "config/id_types.hpp"
#include "side_bar_item.hpp"

class QString;   // Forward declaration
class QMenu;     // Forward declaration
class QAction;   // Forward declaration

namespace ui
{

    /**
     * @brief Item representing an account in the side bar
     *
     */
    class AccountItem : public SideBarItem
    {
       private:
        /// The ID of the account this item represents
        AccountId _id;

        /// action to open the account
        QAction* _openAction;
        /// action to delete the account
        QAction* _deleteAction;

       public:
        explicit AccountItem(AccountId id, const QString& name);

        [[nodiscard]] AccountId getId() const;

        [[nodiscard]] QAction* getOpenAction() const;
        [[nodiscard]] QAction* getDeleteAction() const;

        void populateContextMenu(QMenu& menu) override;
    };
}   // namespace ui

#endif   // __UI__WIDGETS__SIDE_BAR__ACCOUNT_ITEM_HPP__
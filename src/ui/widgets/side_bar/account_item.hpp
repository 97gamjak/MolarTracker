#ifndef __UI__WIDGETS__SIDE_BAR__ACCOUNT_ITEM_HPP__
#define __UI__WIDGETS__SIDE_BAR__ACCOUNT_ITEM_HPP__

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
        // TODO(97gamjak): make this id an AccountId
        /// The ID of the account this item represents
        int _id;

        /// action to open the account
        QAction* _openAction;
        /// action to delete the account
        QAction* _deleteAction;

       public:
        explicit AccountItem(int id, const QString& name);

        [[nodiscard]] int getId() const;

        [[nodiscard]] QAction* getOpenAction() const;
        [[nodiscard]] QAction* getDeleteAction() const;

        void populateContextMenu(QMenu& menu) override;
    };
}   // namespace ui

#endif   // __UI__WIDGETS__SIDE_BAR__ACCOUNT_ITEM_HPP__
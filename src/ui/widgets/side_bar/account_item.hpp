#ifndef __UI__WIDGETS__SIDE_BAR__ACCOUNT_ITEM_HPP__
#define __UI__WIDGETS__SIDE_BAR__ACCOUNT_ITEM_HPP__

#include "side_bar_item.hpp"

class QString;   // Forward declaration
class QMenu;     // Forward declaration
class QAction;   // Forward declaration

namespace ui
{
    class AccountItem : public SideBarItem
    {
       private:
        int _id;

        QAction* _openAction;
        QAction* _deleteAction;

       public:
        explicit AccountItem(int id, const QString& name);

        void populateContextMenu(QMenu& menu) override;
    };
}   // namespace ui

#endif   // __UI__WIDGETS__SIDE_BAR__ACCOUNT_ITEM_HPP__
#ifndef __UI__WIDGETS__SIDE_BAR__ACCOUNT_CATEGORY_HPP__
#define __UI__WIDGETS__SIDE_BAR__ACCOUNT_CATEGORY_HPP__

#include "category.hpp"

class QString;   // Forward declaration
class QMenu;     // Forward declaration
class QAction;   // Forward declaration

namespace ui
{
    class AccountItem;

    class AccountCategory : public Category
    {
       private:
        QAction* _createAction;

       public:
        explicit AccountCategory();

        void addAccount(int id, const QString& name);
        void clearAccounts();

        void populateContextMenu(QMenu& menu) override;
    };
}   // namespace ui

#endif   // __UI__WIDGETS__SIDE_BAR__ACCOUNT_CATEGORY_HPP__
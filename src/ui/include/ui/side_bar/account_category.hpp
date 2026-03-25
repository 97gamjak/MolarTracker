#ifndef __UI__WIDGETS__SIDE_BAR__ACCOUNT_CATEGORY_HPP__
#define __UI__WIDGETS__SIDE_BAR__ACCOUNT_CATEGORY_HPP__

#include "category.hpp"
#include "config/id_types.hpp"

class QString;   // Forward declaration
class QMenu;     // Forward declaration
class QAction;   // Forward declaration

namespace ui
{
    class AccountItem;

    /**
     * @brief Category for accounts in the side bar
     *
     */
    class AccountCategory : public Category
    {
       private:
        /// The action for creating a new account
        QAction* _createAction;

       public:
        explicit AccountCategory();

        void addAccount(AccountId id, const QString& name);
        void clearAccounts();

        [[nodiscard]] QAction* getCreateAction() const;

        void populateContextMenu(QMenu& menu) override;
    };

}   // namespace ui

#endif   // __UI__WIDGETS__SIDE_BAR__ACCOUNT_CATEGORY_HPP__
#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__ACCOUNT_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__ACCOUNT_CONTROLLER_HPP__

#include <QPointer>

#include "config/id_types.hpp"
#include "controller/account_controller.hpp"
#include "side_bar_category_controller.hpp"

namespace cmd
{
    class UndoStack;   // Forward declaration
}   // namespace cmd

namespace ui
{
    class AccountCategory;       // Forward declaration
    class CreateAccountDialog;   // Forward declaration
}   // namespace ui

namespace drafts
{
    class AccountDraft;   // Forward declaration
}   // namespace drafts

namespace store
{
    class IAccountStore;   // Forward declaration
}   // namespace store

namespace cache
{
    class AccountCache;   // Forward declaration
}   // namespace cache

class QAction;   // Forward declaration

namespace controller
{
    class AccountController;   // Forward declaration

    /**
     * @brief Controller for the account category in the side bar
     *
     */
    class AccountSideBarController : public SideBarCategoryController
    {
        Q_OBJECT

       private:
        /// Reference to the undo stack
        cmd::UndoStack& _undoStack;

        /// the account store
        std::shared_ptr<store::IAccountStore> _accountStore;

        /// the account cache
        std::shared_ptr<cache::AccountCache> _accountCache;

        /// the account controller
        AccountController& _accountController;

        /// Pointer to the create account dialog
        QPointer<ui::CreateAccountDialog> _createAccountDialog;

       public:
        explicit AccountSideBarController(
            cmd::UndoStack&                              undoStack,
            const std::shared_ptr<store::IAccountStore>& accountStore,
            const std::shared_ptr<cache::AccountCache>&  accountCache,
            AccountController&                           accountController,
            QMainWindow*                                 mainWindow
        );

        void refresh();

        void handleContextMenuAction(
            const ui::AccountCategory* item,
            const QAction*             action
        );

       public slots:
        void onAccountSelected(AccountId id);

       signals:
        /**
         * @brief Signal emitted when an account is selected in the side bar
         *
         * @param id The ID of the selected account
         */
        void accountSelected(AccountId id);

       private slots:
        void _onCreateAccountRequested(const drafts::AccountDraft& account);
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__ACCOUNT_CONTROLLER_HPP__
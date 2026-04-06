#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__ACCOUNT_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__ACCOUNT_CONTROLLER_HPP__

#include <QPointer>

#include "config/id_types.hpp"
#include "side_bar_category_controller.hpp"

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

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
    struct AccountDraft;   // Forward declaration
}   // namespace drafts

class QAction;   // Forward declaration

namespace controller
{

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
        /// Reference to the application context
        app::AppContext& _appContext;

        /// Pointer to the create account dialog
        QPointer<ui::CreateAccountDialog> _createAccountDialog;

       public:
        explicit AccountSideBarController(
            cmd::UndoStack&  undoStack,
            app::AppContext& appContext,
            QMainWindow*     mainWindow
        );

        void refresh() override;

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
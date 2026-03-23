#ifndef __UI__CONTROLLER__ACCOUNT_CONTROLLER_HPP__
#define __UI__CONTROLLER__ACCOUNT_CONTROLLER_HPP__

#include "side_bar_category_controller.hpp"

class QAction;       // Forward declaration
class QMainWindow;   // Forward declaration

#include <QPointer>

namespace drafts
{
    struct AccountDraft;   // Forward declaration
}   // namespace drafts

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

namespace ui
{
    class UndoStack;             // Forward declaration
    class AccountCategory;       // Forward declaration
    class CreateAccountDialog;   // Forward declaration

    /**
     * @brief Controller for the account category in the side bar
     *
     */
    class AccountSideBarController : public SideBarCategoryController
    {
       private:
        /// Reference to the undo stack
        UndoStack& _undoStack;
        /// Reference to the application context
        app::AppContext& _appContext;

        /// Pointer to the create account dialog
        QPointer<CreateAccountDialog> _createAccountDialog;

       public:
        explicit AccountSideBarController(
            UndoStack&       undoStack,
            app::AppContext& appContext,
            QMainWindow*     mainWindow
        );

        // clang-format off
        ~AccountSideBarController() override = default;
        AccountSideBarController(const AccountSideBarController&)            = delete;
        AccountSideBarController& operator=(const AccountSideBarController&) = delete;
        AccountSideBarController(AccountSideBarController&&)                 = delete;
        AccountSideBarController& operator=(AccountSideBarController&&)      = delete;
        // clang-format on

        void refresh() override;

        void handleContextMenuAction(
            const AccountCategory* item,
            const QAction*         action
        );

       private slots:
        void _onCreateAccountRequested(drafts::AccountDraft account);
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__ACCOUNT_CONTROLLER_HPP__
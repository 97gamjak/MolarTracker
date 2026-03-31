#ifndef __CONTROLLER__SRC__CONTROLLER__ACCOUNT_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__ACCOUNT_CONTROLLER_HPP__

#include <qtmetamacros.h>

#include <QPointer>

#include "config/id_types.hpp"
#include "side_bar_category_controller.hpp"

class QAction;          // Forward declaration
class QMainWindow;      // Forward declaration
class QStackedWidget;   // Forward declaration

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
    class AccountCategory;       // Forward declaration
    class CreateAccountDialog;   // Forward declaration
    class AccountDetailView;     // Forward declaration
}   // namespace ui

namespace cmd
{
    class UndoStack;   // Forward declaration
}   // namespace cmd

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

        // clang-format off
        ~AccountSideBarController() override = default;
        AccountSideBarController(const AccountSideBarController&)            = delete;
        AccountSideBarController& operator=(const AccountSideBarController&) = delete;
        AccountSideBarController(AccountSideBarController&&)                 = delete;
        AccountSideBarController& operator=(AccountSideBarController&&)      = delete;
        // clang-format on

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

    /**
     * @brief Controller for managing account details
     *
     */
    class AccountController : public QObject
    {
        Q_OBJECT

       private:
        /// Reference to the undo stack
        cmd::UndoStack& _undoStack;
        /// Reference to the application context
        app::AppContext& _appContext;
        /// Pointer to the stacked widget
        QStackedWidget* _stackedWidget;
        /// Pointer to the account detail view
        QPointer<ui::AccountDetailView> _accountDetailView;

       public:
        AccountController(
            cmd::UndoStack&           undoStack,
            app::AppContext&          appContext,
            AccountSideBarController& sideBarController,
            QStackedWidget*           stackedWidget
        );

       private slots:
        void _onAccountSelected(AccountId id);
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__ACCOUNT_CONTROLLER_HPP__
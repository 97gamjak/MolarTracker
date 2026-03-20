#ifndef __UI__CONTROLLER__ACCOUNT_CONTROLLER_HPP__
#define __UI__CONTROLLER__ACCOUNT_CONTROLLER_HPP__

#include <QObject>

#include "side_bar_category_controller.hpp"

class QAction;       // Forward declaration
class QMainWindow;   // Forward declaration

namespace drafts
{
    struct AccountDraft;   // Forward declaration
}

namespace app
{
    class AppContext;   // Forward declaration
}

namespace ui
{
    class UndoStack;         // Forward declaration
    class AccountCategory;   // Forward declaration

    /**
     * @brief Controller for the account category in the side bar
     *
     */
    class AccountSideBarController : public SideBarCategoryController
    {
        Q_OBJECT

       private:
        std::shared_ptr<UndoStack>       _undoStack;
        std::shared_ptr<app::AppContext> _appContext;

       public:
        explicit AccountSideBarController(
            UndoStack&       undoStack,
            app::AppContext& appContext,
            QMainWindow*     mainWindow
        );

        void refresh() override;

        void handleContextMenuAction(
            const AccountCategory* item,
            const QAction*         action
        );

       private slots:
        void _onCreateAccountRequested(const drafts::AccountDraft& account);
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__ACCOUNT_CONTROLLER_HPP__
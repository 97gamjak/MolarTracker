#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__

#include <qmainwindow.h>
#include <qtmetamacros.h>

#include <QObject>
#include <QPointer>

#include "side_bar_category_controller.hpp"
#include "ui/transaction/create_transaction_dlg.hpp"

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
    class TransactionCategory;   // Forward declaration
}   // namespace ui

namespace controller
{
    /**
     * @brief Controller for the transaction side bar
     *
     */
    class TransactionSideBarController : public SideBarCategoryController
    {
        Q_OBJECT

       private:
        /// The undo stack for the application
        cmd::UndoStack& _undoStack;
        /// The application context
        app::AppContext& _appContext;

        QPointer<ui::CreateTransactionDialog> _createTransactionDialog;

       public:
        TransactionSideBarController(
            cmd::UndoStack&  undoStack,
            app::AppContext& appContext,
            QMainWindow*     mainWindow
        );

        void refresh() override;

        void handleContextMenuAction(
            const ui::TransactionCategory* item,
            const QAction*                 action
        );
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__

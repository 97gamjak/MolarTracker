#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__

#include <qmainwindow.h>
#include <qtmetamacros.h>

#include <QObject>
#include <QPointer>

#include "config/finance.hpp"
#include "side_bar_category_controller.hpp"
#include "ui/transaction/create_transaction_dlg.hpp"

namespace app
{
    class AccountStore;   // Forward declaration
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
        /// The account store for the application
        app::AccountStore& _accountStore;

        QPointer<ui::CreateTransactionDialog> _createDlg;

       public:
        TransactionSideBarController(
            cmd::UndoStack&    undoStack,
            app::AccountStore& accountStore,
            QMainWindow*       mainWindow
        );

        void refresh() override;

        void handleContextMenuAction(
            const ui::TransactionCategory* item,
            const QAction*                 action
        );

       private slots:
        void _onTransactionTypeChanged(TransactionType type);
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__

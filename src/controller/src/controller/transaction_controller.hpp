#ifndef __CONTROLLER__SRC__CONTROLLER__TRANSACTION_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__TRANSACTION_CONTROLLER_HPP__

#include <qpointer.h>

#include <QObject>

namespace app
{
    class TransactionStore;   // Forward declaration
}   // namespace app

namespace cmd
{
    class UndoStack;   // Forward declaration
}   // namespace cmd

class QStackedWidget;   // Forward declaration

namespace ui
{
    class TransactionsOverview;   // Forward declaration
}   // namespace ui

namespace controller
{
    class TransactionController : public QObject
    {
        Q_OBJECT

       private:
        /// The undo stack for the application
        cmd::UndoStack&        _undoStack;
        app::TransactionStore& _transactionStore;
        /// Pointer to the central stacked widget
        QStackedWidget* _stackedWidget;
        /// Pointer to the transaction detail view
        QPointer<ui::TransactionsOverview> _transactionDetailView;

       public:
        TransactionController(
            cmd::UndoStack&        undoStack,
            app::TransactionStore& transactionStore,
            QStackedWidget*        stackedWidget
        );

        void transactionOverviewSelected();
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__TRANSACTION_CONTROLLER_HPP__
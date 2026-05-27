#ifndef __CONTROLLER__SRC__CONTROLLER__TRANSACTION_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__TRANSACTION_CONTROLLER_HPP__

#include <qpointer.h>

#include <QObject>

namespace store
{
    class TransactionStore;   // Forward declaration
    class AccountStore;       // Forward declaration
    class IStockStore;        // Forward declaration
}   // namespace store

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
    /**
     * @brief Controller for managing transactions and their interactions with
     * the UI and the stores
     *
     */
    class TransactionController : public QObject
    {
        Q_OBJECT

       private:
        /// The undo stack for the application
        cmd::UndoStack& _undoStack;

        /// Reference to the transaction store
        store::TransactionStore& _transactionStore;
        /// Reference to the account store
        store::AccountStore& _accountStore;
        /// Reference to the stock store
        std::shared_ptr<store::IStockStore> _stockStore;

        /// Pointer to the central stacked widget
        QStackedWidget* _stackedWidget;
        /// Pointer to the transaction detail view
        QPointer<ui::TransactionsOverview> _transactionDetailView;

       public:
        TransactionController(
            cmd::UndoStack&                            undoStack,
            store::TransactionStore&                   transactionStore,
            store::AccountStore&                       accountStore,
            const std::shared_ptr<store::IStockStore>& stockStore,
            QStackedWidget*                            stackedWidget
        );

        void transactionOverviewSelected(bool focus);
        void transactionOverviewSelected();
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__TRANSACTION_CONTROLLER_HPP__
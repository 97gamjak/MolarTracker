#ifndef __CONTROLLER__SRC__CONTROLLER__TRANSACTION_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__TRANSACTION_CONTROLLER_HPP__

#include <qpointer.h>

#include <QObject>

namespace store
{
    class ITransactionStore;   // Forward declaration
    class IAccountStore;       // Forward declaration
    class IStockStore;         // Forward declaration
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
        std::shared_ptr<store::ITransactionStore> _transactionStore;
        /// Reference to the account store
        std::shared_ptr<store::IAccountStore> _accountStore;
        /// Reference to the stock store
        std::shared_ptr<store::IStockStore> _stockStore;

        struct UIElements;
        /// A unique pointer to the UI elements used by the controller,
        /// encapsulating the stacked widget and the transaction detail view,
        /// providing a convenient way to manage and access these UI elements
        /// within the controller.
        std::unique_ptr<UIElements> _uiElements;

       public:
        TransactionController(
            cmd::UndoStack&                                  undoStack,
            const std::shared_ptr<store::ITransactionStore>& transactionStore,
            const std::shared_ptr<store::IAccountStore>&     accountStore,
            const std::shared_ptr<store::IStockStore>&       stockStore,
            QStackedWidget*                                  stackedWidget
        );
        ~TransactionController() override;

        void transactionOverviewSelected(bool focus);
        void transactionOverviewSelected();
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__TRANSACTION_CONTROLLER_HPP__
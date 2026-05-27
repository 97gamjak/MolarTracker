#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__

#include <qmainwindow.h>
#include <qtmetamacros.h>

#include <QObject>
#include <QPointer>

#include "connections/connection.hpp"
#include "side_bar_category_controller.hpp"

namespace store
{
    class IAccountStore;      // Forward declaration
    class TransactionStore;   // Forward declaration
    class IStockStore;        // Forward declaration
    class PositionStore;      // Forward declaration
}   // namespace store

namespace cmd
{
    class UndoStack;   // Forward declaration
}   // namespace cmd

namespace ui
{
    class TransactionCategory;       // Forward declaration
    class DepositWithdrawalWidget;   // Forward declaration
    class StockWidget;               // Forward declaration
}   // namespace ui

namespace drafts
{
    class CreateCashTransactionDraft;    // Forward declaration
    class CreateStockTransactionDraft;   // Forward declaration
}   // namespace drafts

namespace finance
{
    class Transaction;   // Forward declaration
}   // namespace finance

class QMainWindow;   // Forward declaration

namespace controller
{
    class TransactionController;         // Forward declaration
    class SecuritiesSideBarController;   // Forward declaration

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
        std::shared_ptr<store::IAccountStore> _accountStore;
        /// The transaction store for the application
        store::TransactionStore& _transactionStore;
        /// The position store for the application
        store::PositionStore& _positionStore;
        /// The stock store for the application
        std::shared_ptr<store::IStockStore> _stockStore;

        /// Pointer to the create transaction dialog
        QPointer<ui::DepositWithdrawalWidget> _createCashTransactionDlg;
        /// Pointer to the stock widget
        QPointer<ui::StockWidget> _createStockTransactionDlg;
        /// Reference to the transaction controller
        TransactionController& _transactionController;
        /// Reference to the stock controller
        SecuritiesSideBarController& _stockController;

        /// Pointer to the main window
        QMainWindow* _mainWindow;

        /// Connections for the transaction side bar controller
        Connections _connections;

       public:
        TransactionSideBarController(
            cmd::UndoStack&                              undoStack,
            const std::shared_ptr<store::IAccountStore>& accountStore,
            store::TransactionStore&                     transactionStore,
            const std::shared_ptr<store::IStockStore>&   stockStore,
            store::PositionStore&                        positionStore,
            TransactionController&                       transactionController,
            SecuritiesSideBarController&                 stockController,
            QMainWindow*                                 mainWindow
        );

        void refresh() override;

        void handleContextMenuAction(
            const ui::TransactionCategory* item,
            const QAction*                 action
        );

        void onTransactionsSelected();

       private:
        void _onCreateCashTransactionRequested(
            drafts::CreateCashTransactionDraft draft
        );

        void _onCreateStockTransactionRequested(
            drafts::CreateStockTransactionDraft draft
        );

        void _onCreateTickerRequested(const std::string& ticker);

        bool _checkAddTransaction(const finance::Transaction& transaction);
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__

#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__

#include <qmainwindow.h>
#include <qtmetamacros.h>

#include <QObject>
#include <QPointer>

#include "side_bar_category_controller.hpp"

namespace app
{
    class AccountStore;       // Forward declaration
    class TransactionStore;   // Forward declaration
    class StockStore;         // Forward declaration
}   // namespace app

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
    class CreateCashTransactionDraft;   // Forward declaration
}   // namespace drafts

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
        app::AccountStore& _accountStore;
        /// The transaction store for the application
        app::TransactionStore& _transactionStore;
        /// The stock store for the application
        app::StockStore& _stockStore;

        /// Pointer to the create transaction dialog
        QPointer<ui::DepositWithdrawalWidget> _createCashTransactionDlg;
        /// Pointer to the stock widget
        QPointer<ui::StockWidget> _createStockTransactionDlg;
        /// Reference to the transaction controller
        TransactionController&       _transactionController;
        SecuritiesSideBarController& _stockController;

        QMainWindow* _mainWindow;

       public:
        TransactionSideBarController(
            cmd::UndoStack&              undoStack,
            app::AccountStore&           accountStore,
            app::TransactionStore&       transactionStore,
            app::StockStore&             stockStore,
            TransactionController&       transactionController,
            SecuritiesSideBarController& stockController,
            QMainWindow*                 mainWindow
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

        void _onCreateTickerRequested(const std::string& ticker);
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__

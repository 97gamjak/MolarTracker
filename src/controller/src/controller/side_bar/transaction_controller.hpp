#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__

#include <qmainwindow.h>
#include <qtmetamacros.h>

#include <QObject>
#include <QPointer>
#include <cstdint>

#include "side_bar_category_controller.hpp"

namespace store
{
    class IAccountStore;                                // Forward declaration
    class ITransactionStore;                            // Forward declaration
    class IStockStore;                                  // Forward declaration
    class IOptionStore;                                 // Forward declaration
    class IPositionStore;                               // Forward declaration
    enum class TransactionStoreResult : std::uint8_t;   // Forward declaration
}   // namespace store

namespace cmd
{
    class UndoStack;   // Forward declaration
}   // namespace cmd

namespace gateway
{
    class PositionGateway;   // Forward declaration
}   // namespace gateway

namespace ui
{
    class TransactionCategory;       // Forward declaration
    class DepositWithdrawalWidget;   // Forward declaration
    class StockWidget;               // Forward declaration
    class OptionWidget;              // Forward declaration
}   // namespace ui

namespace drafts
{
    class CreateCashTransactionDraft;     // Forward declaration
    class CreateStockTransactionDraft;    // Forward declaration
    class CreateOptionTransactionDraft;   // Forward declaration
}   // namespace drafts

namespace finance
{
    class DomainTransaction;   // Forward declaration
}   // namespace finance

class QMainWindow;   // Forward declaration
class Connections;   // Forward declaration

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
        /// The position gateway for fetching position-related data
        std::shared_ptr<gateway::PositionGateway> _positionGateway;
        /// The account store for the application
        std::shared_ptr<store::IAccountStore> _accountStore;
        /// The transaction store for the application
        std::shared_ptr<store::ITransactionStore> _transactionStore;
        /// The position store for the application
        std::shared_ptr<store::IPositionStore> _positionStore;
        /// The stock store for the application
        std::shared_ptr<store::IStockStore> _stockStore;
        /// The option store for the application
        std::shared_ptr<store::IOptionStore> _optionStore;

        struct Dialogs;
        /// Pointer to the dialogs struct
        std::unique_ptr<Dialogs> _dialogs;

        /// Reference to the transaction controller
        TransactionController& _transactionController;
        /// Reference to the stock controller
        SecuritiesSideBarController& _stockController;

        /// Connections for the transaction side bar controller
        std::unique_ptr<Connections> _connections;

       public:
        TransactionSideBarController(
            cmd::UndoStack&                                  undoStack,
            const std::shared_ptr<gateway::PositionGateway>& positionGateway,
            const std::shared_ptr<store::IAccountStore>&     accountStore,
            const std::shared_ptr<store::ITransactionStore>& transactionStore,
            const std::shared_ptr<store::IStockStore>&       stockStore,
            const std::shared_ptr<store::IOptionStore>&      optionStore,
            const std::shared_ptr<store::IPositionStore>&    positionStore,
            TransactionController&       transactionController,
            SecuritiesSideBarController& stockController,
            QMainWindow*                 mainWindow
        );
        ~TransactionSideBarController() override;

        void refresh() override;

        void handleContextMenuAction(
            const ui::TransactionCategory* item,
            const QAction*                 action
        );

        void onTransactionsSelected();

       private:
        void _onCreateCashTransactionRequested(
            const drafts::CreateCashTransactionDraft& draft
        );

        void _onCreateStockTransactionRequested(
            drafts::CreateStockTransactionDraft draft
        );

        void _onCreateOptionTransactionRequested(
            drafts::CreateOptionTransactionDraft draft
        );

        void _onCreateTickerRequested(const std::string& ticker);

        static bool _checkAddTransaction(store::TransactionStoreResult result);
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__TRANSACTION_CONTROLLER_HPP__

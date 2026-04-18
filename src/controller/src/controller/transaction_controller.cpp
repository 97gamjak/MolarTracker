#include "transaction_controller.hpp"

#include <qstackedwidget.h>

#include "app/store/transaction_store.hpp"
#include "ui/transaction/transactions_overview.hpp"

namespace controller
{

    TransactionController::TransactionController(
        cmd::UndoStack&        undoStack,
        app::TransactionStore& transactionStore,
        QStackedWidget*        stackedWidget
    )
        : _undoStack(undoStack),
          _transactionStore(transactionStore),
          _stackedWidget(stackedWidget),
          _transactionDetailView(new ui::TransactionsOverview(_stackedWidget))
    {
        _stackedWidget->addWidget(_transactionDetailView);
    }

    void TransactionController::transactionOverviewSelected()
    {
        _stackedWidget->setCurrentWidget(_transactionDetailView);
        const auto transaction = _transactionStore.getTransactions();
        _transactionDetailView->refresh(transaction);
    }
}   // namespace controller
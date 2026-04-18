#include "transaction_controller.hpp"

#include <qstackedwidget.h>

#include "app/store/account_store.hpp"
#include "app/store/transaction_store.hpp"
#include "ui/transaction/transactions_overview.hpp"

namespace controller
{

    TransactionController::TransactionController(
        cmd::UndoStack&        undoStack,
        app::TransactionStore& transactionStore,
        app::AccountStore&     accountStore,
        QStackedWidget*        stackedWidget
    )
        : _undoStack(undoStack),
          _transactionStore(transactionStore),
          _accountStore(accountStore),
          _stackedWidget(stackedWidget),
          _transactionDetailView(new ui::TransactionsOverview(_stackedWidget))
    {
        _stackedWidget->addWidget(_transactionDetailView);
    }

    void TransactionController::transactionOverviewSelected()
    {
        _stackedWidget->setCurrentWidget(_transactionDetailView);

        _transactionDetailView->refresh(
            _transactionStore.getTransactions(),
            _accountStore.getAccountIdToNameMap()
        );
    }
}   // namespace controller
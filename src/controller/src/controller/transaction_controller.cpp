#include "transaction_controller.hpp"

#include <qstackedwidget.h>

#include "app/store/account_store.hpp"
#include "app/store/transaction_store.hpp"
#include "ui/transaction/transactions_overview.hpp"

namespace controller
{

    /**
     * @brief Construct a new Transaction Controller:: Transaction Controller
     * object
     *
     * @param undoStack
     * @param transactionStore
     * @param accountStore
     * @param stackedWidget
     */
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

    /**
     * @brief Handle the selection of the transaction overview, this will be
     * called when the transaction overview is selected in the UI (e.g. when the
     * user clicks on the transaction category in the side bar), and should
     * handle updating the transaction overview with the latest transactions
     * from the store, as well as any necessary account information (e.g.
     * account names for displaying in the overview). This allows the controller
     * to ensure that the transaction overview is always up to date with the
     * latest data from the store, and provides a way for the UI to trigger
     * updates to the transaction overview when it is selected.
     *
     */
    void TransactionController::transactionOverviewSelected()
    {
        transactionOverviewSelected(true);
    }

    /**
     * @brief Handle the selection of the transaction overview, this will be
     * called when the transaction overview is selected in the UI (e.g. when the
     * user clicks on the transaction category in the side bar), and should
     * handle updating the transaction overview with the latest transactions
     * from the store, as well as any necessary account information (e.g.
     * account names for displaying in the overview). This allows the controller
     * to ensure that the transaction overview is always up to date with the
     * latest data from the store, and provides a way for the UI to trigger
     * updates to the transaction overview when it is selected.
     *
     * @param focus A boolean indicating whether to focus on the transaction
     * overview after updating it, if true, this will set the current widget of
     * the stacked widget to the transaction overview, allowing it to be visible
     * to the user after it has been updated with the latest data.
     */
    void TransactionController::transactionOverviewSelected(bool focus)
    {
        if (focus)
            _stackedWidget->setCurrentWidget(_transactionDetailView);

        _transactionDetailView->refresh(
            _transactionStore.getTransactions(),
            _accountStore.getAccountIdToNameMap()
        );
    }
}   // namespace controller
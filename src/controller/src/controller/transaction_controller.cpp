#include "transaction_controller.hpp"

#include <qstackedwidget.h>

#include "cache/stock_cache.hpp"
#include "controller/mapper/transaction/transaction_overview_mapper.hpp"
#include "store/i_transaction_store.hpp"
#include "ui/transaction/transactions_overview.hpp"
#include "ui/utils/error.hpp"

namespace controller
{

    /**
     * @brief Construct a new Transaction Controller:: Transaction Controller
     * object
     *
     * @param undoStack
     * @param transactionStore
     * @param accountCache
     * @param stockCache
     * @param stackedWidget
     */
    TransactionController::TransactionController(
        cmd::UndoStack&                                  undoStack,
        const std::shared_ptr<store::ITransactionStore>& transactionStore,
        const std::shared_ptr<cache::AccountCache>&      accountCache,
        const std::shared_ptr<cache::StockCache>&        stockCache,
        QStackedWidget*                                  stackedWidget
    )
        : _undoStack(undoStack),
          _transactionStore(transactionStore),
          _accountCache(accountCache),
          _stockCache(stockCache),
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

        const auto transactions = _transactionStore->getTransactions();

        const auto cashDrafts =
            TransactionOverviewMapper::toCash(transactions, _accountCache);

        if (!cashDrafts)
        {
            ui::ErrorDialog::show(
                cashDrafts.error(),
                "Could not load cash transactions for the transaction overview",
                _transactionDetailView
            );
            return;
        }

        const auto stockDrafts = TransactionOverviewMapper::toStockOverview(
            transactions,
            _stockCache,
            _accountCache
        );

        if (!stockDrafts)
        {
            ui::ErrorDialog::show(
                stockDrafts.error(),
                "Could not load stock transactions for the transaction "
                "overview",
                _transactionDetailView
            );
            return;
        }

        _transactionDetailView->refresh(*cashDrafts, *stockDrafts);
    }
}   // namespace controller
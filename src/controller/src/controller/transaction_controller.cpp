#include "transaction_controller.hpp"

#include <qstackedwidget.h>

#include "logging/log_macros.hpp"
#include "mapper/transaction/transaction_overview_mapper.hpp"
#include "store/i_account_store.hpp"
#include "store/i_option_store.hpp"
#include "store/i_stock_store.hpp"
#include "store/i_transaction_store.hpp"
#include "ui/transaction/transactions_overview.hpp"
#include "ui/utils/error.hpp"

REGISTER_LOG_CATEGORY("Controller.TransactionController");

namespace controller
{
    /**
     * @brief A struct to hold the UI elements for the TransactionController,
     * this struct encapsulates the UI elements used by the
     * TransactionController, including the stacked widget and the transaction
     * detail view, providing a convenient way to manage and access these UI
     * elements within the controller.
     *
     */
    struct TransactionController::UIElements
    {
        /// Pointer to the central stacked widget
        QStackedWidget* stackedWidget;
        /// Pointer to the transaction detail view
        QPointer<ui::TransactionsOverview> transactionDetailView;

        explicit UIElements(QStackedWidget* stackedWidget_);
    };

    /**
     * @brief Construct a new UIElements object
     *
     * @param stackedWidget_ Pointer to the central stacked widget
     */
    TransactionController::UIElements::UIElements(
        QStackedWidget* stackedWidget_
    )
        : stackedWidget(stackedWidget_),
          transactionDetailView(new ui::TransactionsOverview(stackedWidget))
    {
        stackedWidget->addWidget(transactionDetailView);
    }

    /**
     * @brief Construct a new Transaction Controller:: Transaction Controller
     * object
     *
     * @param undoStack
     * @param transactionStore
     * @param accountStore
     * @param stockStore
     * @param optionStore
     * @param stackedWidget
     */
    TransactionController::TransactionController(
        cmd::UndoStack&                                  undoStack,
        const std::shared_ptr<store::ITransactionStore>& transactionStore,
        const std::shared_ptr<store::IAccountStore>&     accountStore,
        const std::shared_ptr<store::IStockStore>&       stockStore,
        const std::shared_ptr<store::IOptionStore>&      optionStore,
        QStackedWidget*                                  stackedWidget
    )
        : _undoStack(undoStack),
          _transactionStore(transactionStore),
          _accountStore(accountStore),
          _stockStore(stockStore),
          _optionStore(optionStore),
          _uiElements(std::make_unique<UIElements>(stackedWidget))
    {
    }

    TransactionController::~TransactionController() = default;

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
            _uiElements->stackedWidget->setCurrentWidget(
                _uiElements->transactionDetailView
            );

        const auto txs = _transactionStore->getTransactions();

        if (!txs)
        {
            LOG_ERROR(txs.error().toString());
            ui::ErrorDialog::show(
                txs.error(),
                "Failed to retrieve transactions for overview",
                _uiElements->stackedWidget
            );
        }

        const auto cashDrafts =
            mapper::TransactionOverviewMapper::toCash(txs.value());
        const auto stockDrafts = mapper::TransactionOverviewMapper::toStock(
            txs.value(),
            _stockStore->getInstrumentIdToNameMap()
        );
        const auto optionDrafts = mapper::TransactionOverviewMapper::toOption(
            txs.value(),
            _optionStore->getOptions(txs.value().getOptionInstrumentIds())
        );

        _uiElements->transactionDetailView->refresh(
            cashDrafts,
            stockDrafts,
            optionDrafts,
            _accountStore->getAccountIdToNameMap()
        );
    }
}   // namespace controller
#include "transaction_controller.hpp"

#include <stdexcept>

#include "app/store/account_store.hpp"
#include "app/store/stock_store.hpp"
#include "app/store/transaction_store.hpp"
#include "config/finance.hpp"
#include "controller/side_bar/securities_controller.hpp"
#include "controller/transaction_controller.hpp"
#include "drafts/transaction_draft.hpp"
#include "drafts/transaction_mapper.hpp"
#include "logging/log_macros.hpp"
#include "ui/side_bar/transaction_category.hpp"
#include "ui/transaction/deposit_withdrawal_widget.hpp"
#include "ui/transaction/stock_widget.hpp"
#include "utils/qt_helpers.hpp"

REGISTER_LOG_CATEGORY("Controller.SideBar.TransactionSideBarController");

namespace controller
{
    /**
     * @brief Construct a new Transaction Side Bar Controller:: Transaction Side
     * Bar Controller object
     *
     * @param undoStack The undo stack for the application
     * @param accountStore The account store for the application
     * @param transactionStore The transaction store for the application
     * @param transactionController The transaction controller for the
     * application
     * @param mainWindow The main window of the application
     */
    TransactionSideBarController::TransactionSideBarController(
        cmd::UndoStack&              undoStack,
        app::AccountStore&           accountStore,
        app::TransactionStore&       transactionStore,
        app::StockStore&             stockStore,
        TransactionController&       transactionController,
        SecuritiesSideBarController& stockController,
        QMainWindow*                 mainWindow
    )
        : SideBarCategoryController(new ui::TransactionCategory(), mainWindow),
          _undoStack(undoStack),
          _accountStore(accountStore),
          _transactionStore(transactionStore),
          _stockStore(stockStore),
          _createCashTransactionDlg(nullptr),
          _createStockTransactionDlg(nullptr),
          _transactionController(transactionController),
          _stockController(stockController),
          _mainWindow(mainWindow)
    {
    }

    /**
     * @brief Refresh the transaction category
     *
     */
    void TransactionSideBarController::refresh() {}

    /**
     * @brief Handle a context menu action for the transaction category
     *
     * @param item The transaction category item
     * @param action The action that was triggered
     */
    void TransactionSideBarController::handleContextMenuAction(
        const ui::TransactionCategory* item,
        const QAction*                 action
    )
    {
        if (item == nullptr || action == nullptr)
        {
            LOG_WARNING(
                "Context menu requested with null item or action, ignoring"
            );
            return;
        }

        if (action == item->getCreateDepositAction() ||
            action == item->getCreateWithdrawalAction())
        {
            const auto type = action->data().value<TransactionType>();

            LOG_DEBUG(
                "Create action triggered for transaction category with type: " +
                TransactionTypeMeta::toString(type)
            );

            if (_createCashTransactionDlg == nullptr)
            {
                _createCashTransactionDlg =
                    utils::makeQChild<ui::DepositWithdrawalWidget>(
                        type,
                        _accountStore.getCashAccounts(),
                        _mainWindow
                    );

                connect(
                    _createCashTransactionDlg,
                    &ui::DepositWithdrawalWidget::
                        createCashTransactionRequested,
                    this,
                    &TransactionSideBarController::
                        _onCreateCashTransactionRequested
                );
            }
            else
            {
                _createCashTransactionDlg->setTransactionType(type);
                _createCashTransactionDlg->updateAccounts(
                    _accountStore.getCashAccounts()
                );
                _createCashTransactionDlg->refresh();
            }

            _createCashTransactionDlg->show();
        }
        else if (action == item->getCreateStockTransactionAction())
        {
            if (_createStockTransactionDlg == nullptr)
            {
                _createStockTransactionDlg = utils::makeQChild<ui::StockWidget>(
                    _accountStore.getAllAccounts(),
                    _accountStore.getAllAccounts(),
                    _stockStore.getAllTickers(),
                    _mainWindow
                );

                connect(
                    _createStockTransactionDlg,
                    &ui::StockWidget::createTickerRequested,
                    this,
                    &TransactionSideBarController::_onCreateTickerRequested
                );
            }
            else
            {
                _createStockTransactionDlg->updateAccounts(
                    _accountStore.getAllAccounts()
                );
                _createStockTransactionDlg->updateReferenceAccounts(
                    _accountStore.getAllAccounts()
                );
                _createStockTransactionDlg->updateTickers(
                    _stockStore.getAllTickers()
                );
                _createStockTransactionDlg->refresh();
            }

            _createStockTransactionDlg->show();
        }
        else
        {
            throw std::logic_error(
                "Unhandled context menu action for transaction category: " +
                action->text().toStdString()
            );
        }
    }

    /**
     * @brief Handle the creation of a new cash transaction, this will be called
     * when the user submits the create transaction dialog for a cash
     * transaction, and should handle validating the transaction draft, adding
     * any necessary additional entries (e.g. for external accounts), and then
     * adding the transaction to the store. This allows the controller to manage
     * the process of creating a new cash transaction from the UI, ensuring that
     * the transaction is properly validated and added to the store with all
     * necessary information.
     *
     * @param draft The draft of the cash transaction to create, this contains
     * all the necessary information for creating a new cash transaction,
     * including the timestamp, entries, and any optional comment.
     */
    void TransactionSideBarController::_onCreateCashTransactionRequested(
        drafts::CreateCashTransactionDraft draft
    )
    {
        std::vector<drafts::TransactionEntryDraft> additionalEntries;

        for (auto entry : draft.getEntries())
        {
            if (entry.needsExternal())
            {
                additionalEntries.emplace_back(
                    _accountStore.getExternalAccount(
                        entry.getCash().getCurrency()
                    ),
                    -entry.getCash()
                );
            }
        }

        for (const auto& entry : additionalEntries)
            draft.addEntry(entry);

        _transactionStore.addTransaction(
            drafts::TransactionMapper::fromCreateCashTransactionDraft(draft)
        );

        // TODO(97gamjak): add here commands and also error handling
        _createCashTransactionDlg->close();
        _transactionController.transactionOverviewSelected(false);
    }

    /**
     * @brief Handle the selection of transactions in the side bar,
     * this will trigger the transaction overview to update with the
     * latest data from the store, and provides a way for the UI to
     * trigger updates to the transaction overview when it is
     * selected.
     *
     */
    void TransactionSideBarController::onTransactionsSelected()
    {
        _transactionController.transactionOverviewSelected();
    }

    void TransactionSideBarController::_onCreateTickerRequested(
        const std::string& ticker
    )
    {
        _stockController.createStock(ticker);
        _createStockTransactionDlg->updateTickers(
            _stockStore.getAllTickers(),
            ticker
        );
    }

}   // namespace controller

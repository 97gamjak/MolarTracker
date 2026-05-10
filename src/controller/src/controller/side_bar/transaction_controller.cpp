#include "transaction_controller.hpp"

#include <stdexcept>

#include "app/store/account_store.hpp"
#include "app/store/position_store.hpp"
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
     * @param stockStore The stock store for the application
     * @param positionStore The position store for the application
     * @param transactionController The transaction controller for the
     * application
     * @param stockController The stock controller for the application
     * @param mainWindow The main window of the application
     */
    TransactionSideBarController::TransactionSideBarController(
        cmd::UndoStack&              undoStack,
        app::AccountStore&           accountStore,
        app::TransactionStore&       transactionStore,
        app::StockStore&             stockStore,
        app::PositionStore&          positionStore,
        TransactionController&       transactionController,
        SecuritiesSideBarController& stockController,
        QMainWindow*                 mainWindow
    )
        : SideBarCategoryController(new ui::TransactionCategory(), mainWindow),
          _undoStack(undoStack),
          _accountStore(accountStore),
          _transactionStore(transactionStore),
          _positionStore(positionStore),
          _stockStore(stockStore),
          _createCashTransactionDlg(nullptr),
          _createStockTransactionDlg(nullptr),
          _transactionController(transactionController),
          _stockController(stockController),
          _mainWindow(mainWindow)
    {
        _createCashTransactionDlg =
            utils::makeQChild<ui::DepositWithdrawalWidget>(
                TransactionType::Deposit,   // dummy type
                _accountStore.getCashAccounts(),
                _mainWindow
            );

        connect(
            _createCashTransactionDlg,
            &ui::DepositWithdrawalWidget::createCashTransactionRequested,
            this,
            &TransactionSideBarController::_onCreateCashTransactionRequested
        );

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

        connect(
            _createStockTransactionDlg,
            &ui::StockWidget::createStockTransactionRequested,
            this,
            &TransactionSideBarController::_onCreateStockTransactionRequested
        );

        _connections.add(_stockStore.subscribeToStoreChange(
            [&]()
            {
                _createStockTransactionDlg->updateTickers(
                    _stockStore.getAllTickers()
                );
            },
            this
        ));
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

            _createCashTransactionDlg->setTransactionType(type);
            _createCashTransactionDlg->updateAccounts(
                _accountStore.getCashAccounts()
            );
            _createCashTransactionDlg->refresh();

            _createCashTransactionDlg->show();
        }
        else if (action == item->getCreateStockTransactionAction())
        {
            _createStockTransactionDlg->updateAccounts(
                _accountStore.getSecurityAccounts()
            );
            _createStockTransactionDlg->updateReferenceAccounts(
                _accountStore.getCashAccounts()
            );
            _createStockTransactionDlg->updateTickers(_stockStore.getAllTickers(
            ));
            _createStockTransactionDlg->refresh();

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
        LOG_ENTRY;

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
     * @brief Handle the creation of a new stock transaction, this will be
     * called when the user submits the create transaction dialog for a stock
     * transaction, and should handle validating the transaction draft, adding
     * any necessary additional entries (e.g. for external accounts), and then
     * adding the transaction to the store. This allows the controller to manage
     * the process of creating a new stock transaction from the UI, ensuring
     * that the transaction is properly validated and added to the store with
     * all necessary information.
     *
     * @param draft The draft of the stock transaction to create, this contains
     * all the necessary information for creating a new stock transaction,
     * including the timestamp, legs, and any optional comment.
     */
    void TransactionSideBarController::_onCreateStockTransactionRequested(
        drafts::CreateStockTransactionDraft draft
    )
    {
        LOG_ENTRY;

        for (auto& leg : draft.getLegs())
        {
            const auto instrumentId =
                _stockStore.getInstrumentId(leg.getTicker());

            if (instrumentId.has_value())
            {
                leg.setInstrumentId(*instrumentId);
            }
            else
            {
                throw std::logic_error(
                    "Invalid stock ticker: " + leg.getTicker()
                );
            }
        }

        const auto openPositions = _positionStore.getAllPositions();
        std::vector<PositionId> positionIds;
        for (const auto& position : openPositions)
            positionIds.push_back(position.getId());

        const auto instrumentIds =
            _transactionStore.getInstrumentIdsByPositionId(positionIds);
        // TODO: ask if position is open if it should be added to an existing
        // position or if a new position should be created

        auto       position   = finance::Position(draft.getTimestamp());
        const auto positionId = _positionStore.createPosition(position);

        for (auto& leg : draft.getLegs())
            leg.setPositionId(positionId);

        _transactionStore.addTransaction(
            drafts::TransactionMapper::fromCreateStockTransactionDraft(draft)
        );

        // TODO(97gamjak): add here commands and also error handling
        _createStockTransactionDlg->close();
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

    /**
     * @brief Handle the creation of a new stock ticker
     *
     * @param ticker The ticker symbol of the stock to create
     */
    void TransactionSideBarController::_onCreateTickerRequested(
        const std::string& ticker
    )
    {
        _stockController.createStock(ticker);
    }

}   // namespace controller

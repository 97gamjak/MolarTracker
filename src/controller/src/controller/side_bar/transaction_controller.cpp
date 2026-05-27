#include "transaction_controller.hpp"

#include <stdexcept>

#include "config/constants.hpp"
#include "config/finance.hpp"
#include "controller/side_bar/securities_controller.hpp"
#include "controller/transaction/transaction_helpers.hpp"
#include "controller/transaction_controller.hpp"
#include "drafts/account_mapper.hpp"
#include "drafts/position_draft.hpp"
#include "drafts/stock_mapper.hpp"
#include "drafts/transaction_draft.hpp"
#include "drafts/transaction_mapper.hpp"
#include "logging/log_macros.hpp"
#include "store/i_account_store.hpp"
#include "store/position_store.hpp"
#include "store/stock_store.hpp"
#include "store/transaction_store.hpp"
#include "ui/position/position_selection_dialog.hpp"
#include "ui/side_bar/transaction_category.hpp"
#include "ui/transaction/deposit_withdrawal_widget.hpp"
#include "ui/transaction/stock_widget.hpp"
#include "ui/utils/error.hpp"
#include "utils/qt_helpers.hpp"

REGISTER_LOG_CATEGORY("Controller.SideBar.TransactionSideBarController");

using drafts::TransactionMapper;
using finance::Position;

using store::IAccountStore;
using store::PositionStore;
using store::StockStore;
using store::TransactionStore;
using store::TransactionStoreResult;
using store::TransactionStoreResultMeta;

using ui::DepositWithdrawalWidget;
using ui::ErrorDialog;
using ui::PositionSelectionDialog;
using ui::StockWidget;
using ui::TransactionCategory;

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
        cmd::UndoStack&                       undoStack,
        const std::shared_ptr<IAccountStore>& accountStore,
        TransactionStore&                     transactionStore,
        StockStore&                           stockStore,
        PositionStore&                        positionStore,
        TransactionController&                transactionController,
        SecuritiesSideBarController&          stockController,
        QMainWindow*                          mainWindow
    )
        : SideBarCategoryController(new TransactionCategory(), mainWindow),
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
        const auto cashAccounts =
            drafts::AccountMapper::toDrafts(_accountStore->getCashAccounts());
        _createCashTransactionDlg = utils::makeQChild<DepositWithdrawalWidget>(
            TransactionType::Deposit,   // dummy type
            cashAccounts,
            _mainWindow
        );

        connect(
            _createCashTransactionDlg,
            &DepositWithdrawalWidget::createCashTransactionRequested,
            this,
            &TransactionSideBarController::_onCreateCashTransactionRequested
        );

        const auto accounts =
            drafts::AccountMapper::toDrafts(_accountStore->getAllAccounts());

        _createStockTransactionDlg = utils::makeQChild<StockWidget>(
            accounts,
            accounts,
            _stockStore.getAllTickers(),
            _mainWindow
        );

        connect(
            _createStockTransactionDlg,
            &StockWidget::createTickerRequested,
            this,
            &TransactionSideBarController::_onCreateTickerRequested
        );

        connect(
            _createStockTransactionDlg,
            &StockWidget::createStockTransactionRequested,
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
        const TransactionCategory* item,
        const QAction*             action
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
                drafts::AccountMapper::toDrafts(_accountStore->getCashAccounts()
                )
            );
            _createCashTransactionDlg->refresh();

            _createCashTransactionDlg->show();
        }
        else if (action == item->getCreateStockTransactionAction())
        {
            _createStockTransactionDlg->updateAccounts(
                drafts::AccountMapper::toDrafts(
                    _accountStore->getSecurityAccounts()
                )
            );
            _createStockTransactionDlg->updateReferenceAccounts(
                drafts::AccountMapper::toDrafts(_accountStore->getCashAccounts()
                )
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

    namespace
    {
        /**
         * @brief Add external transaction entries for any entries that need
         * them
         *
         * @param draft The list of transaction entries to modify
         * @param accountStore The account store to use for looking up external
         * accounts
         *
         * @return true if external transaction entries were added, false
         * otherwise
         */
        [[nodiscard]]
        bool addExternalTransactionEntries(
            drafts::CreateTransactionDraft*              draft,
            const std::shared_ptr<store::IAccountStore>& accountStore
        )
        {
            std::vector<drafts::TransactionEntryDraft> additionalEntries;

            for (const auto& entry : draft->getEntries())
            {
                if (entry.needsExternal() ||
                    entry.getType() != TransactionEntryType::General)
                {
                    const auto currency = entry.getCash().getCurrency();
                    const auto accountId =
                        accountStore->getExternalAccount(currency);

                    if (!accountId.has_value())
                        return false;

                    additionalEntries.emplace_back(
                        accountId.value(),
                        -entry.getCash(),
                        entry.getType(),
                        true
                    );
                }
            }

            for (const auto& entry : additionalEntries)
                draft->addEntry(entry);

            return true;
        }
    }   // namespace

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

        if (!addExternalTransactionEntries(&draft, _accountStore))
        {
            const std::string msg =
                "Failed to add external transaction entries";
            LOG_ERROR(msg);
            ErrorDialog::show(msg);
        }

        const auto transaction =
            TransactionMapper::fromCreateCashTransactionDraft(draft);

        if (!_checkAddTransaction(transaction))
            return;

        // TODO(97gamjak): add here commands
        _createCashTransactionDlg->close();

        // TODO(97gamjak): handle this via notifications and not explicitly
        // here
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

        const auto result = convertTickerToInstrumentId(draft, _stockStore);
        if (!result)
            throw std::logic_error(result.error());

        const auto openPositions = _positionStore.getOpenPositions();
        std::vector<PositionId> positionIds;
        for (const auto& position : openPositions)
            positionIds.push_back(position.getId());

        std::vector<drafts::PositionDraft> drafts;
        for (const auto& position : openPositions)
        {
            const auto instrumentIds =
                _transactionStore.getInstrumentIdsByPositionId(position.getId()
                );

            const auto& stocks = _stockStore.getStocks(instrumentIds);

            if (stocks.empty())
            {
                LOG_WARNING(
                    "No stock found for instrument id: " +
                    position.getId().toString()
                );
                continue;
            }

            drafts.emplace_back(
                position.getId(),
                drafts::StockMapper::toStockInfoDraft(stocks.front()),
                position.getCreatedAt()
            );
        }

        PositionId positionId = PositionId::invalid();
        if (drafts.size() > 0)
        {
            PositionSelectionDialog dlg{drafts};
            if (dlg.exec() == QDialog::Accepted)
            {
                if (auto pos = dlg.selectedPosition())
                    positionId = pos->getPositionId();
            }
            else
            {
                LOG_INFO("No position selected");
                return;
            }
        }

        if (!positionId.isValid())
        {
            auto position = Position(draft.getTimestamp());
            positionId    = _positionStore.createPosition(position);
        }

        for (auto& leg : draft.getLegs())
            leg.setPositionId(positionId);

        if (!addExternalTransactionEntries(&draft, _accountStore))
        {
            const std::string msg =
                "Failed to add external transaction entries";
            LOG_ERROR(msg);
            ErrorDialog::show(msg);
        }

        const auto transaction =
            TransactionMapper::fromCreateStockTransactionDraft(draft);

        if (!_checkAddTransaction(transaction))
            return;

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

    /**
     * @brief Check if a transaction can be added
     *
     * @param transaction The transaction to check
     * @return true if the transaction can be added, false otherwise
     */
    bool TransactionSideBarController::_checkAddTransaction(
        const finance::Transaction& transaction
    )
    {
        // Check if the transaction can be added
        const auto result = _transactionStore.addTransaction(transaction);

        switch (result)
        {
            case TransactionStoreResult::Ok:
                return true;

            case TransactionStoreResult::Error:
            case TransactionStoreResult::TransactionSumNotZero:
            {
                const auto msg = "Failed to create cash transaction: " +
                                 TransactionStoreResultMeta::toString(result) +
                                 ". " + Constants::getCreateIssueError();

                LOG_ERROR(msg);
                ErrorDialog::show(msg);
                return false;
            }
        }

        std::unreachable();
    }

}   // namespace controller

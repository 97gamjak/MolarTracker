#include "transaction_controller.hpp"

#include <stdexcept>
#include <string>

#include "config/constants/github_constants.hpp"
#include "connections/connection.hpp"
#include "controller/helpers.hpp"
#include "controller/mapper/account_mapper.hpp"
#include "controller/mapper/option_mapper.hpp"
#include "controller/mapper/transaction/transaction_create_mapper.hpp"
#include "controller/side_bar/securities_controller.hpp"
#include "controller/transaction_controller.hpp"
#include "drafts/account_draft.hpp"
#include "drafts/position_draft.hpp"
#include "drafts/transaction/transaction_create_draft.hpp"
#include "finance/position.hpp"
#include "logging/log_macros.hpp"
#include "store/i_account_store.hpp"
#include "store/i_option_store.hpp"
#include "store/i_position_store.hpp"
#include "store/i_stock_store.hpp"
#include "store/i_transaction_store.hpp"
#include "ui/position/position_selection_dialog.hpp"
#include "ui/side_bar/transaction_category.hpp"
#include "ui/transaction/deposit_withdrawal_widget.hpp"
#include "ui/transaction/option_widget.hpp"
#include "ui/transaction/stock_widget.hpp"
#include "ui/utils/error.hpp"
#include "utils/finance.hpp"
#include "utils/qt_helpers.hpp"

REGISTER_LOG_CATEGORY("Controller.SideBar.TransactionSideBarController");

using finance::Position;

using store::IAccountStore;
using store::IOptionStore;
using store::IPositionStore;
using store::IStockStore;
using store::ITransactionStore;
using store::TransactionStoreResult;
using store::TransactionStoreResultMeta;

using ui::DepositWithdrawalWidget;
using ui::ErrorDialog;
using ui::OptionWidget;
using ui::PositionSelectionDialog;
using ui::StockWidget;
using ui::TransactionCategory;

namespace controller
{
    /**
     * @brief Struct for holding the dialogs in the transaction side bar
     * controller
     *
     */
    struct TransactionSideBarController::Dialogs
    {
        /// Dialog for creating cash transactions
        QPointer<ui::DepositWithdrawalWidget> cash = nullptr;
        /// Dialog for creating stock transactions
        QPointer<ui::StockWidget> stock = nullptr;
        /// Dialog for creating option transactions
        QPointer<ui::OptionWidget> option = nullptr;

        Dialogs(
            const std::vector<drafts::AccountDraft>& cashAccounts,
            const std::vector<drafts::AccountDraft>& securityAccounts,
            const Set<std::string>&                  tickers,
            QMainWindow*                             mainWindow
        );
    };

    /**
     * @brief Construct a new Transaction Side Bar Controller:: Dialogs::
     * Dialogs object
     *
     * @param cashAccounts
     * @param securityAccounts
     * @param tickers
     * @param mainWindow
     */
    TransactionSideBarController::Dialogs::Dialogs(
        const std::vector<drafts::AccountDraft>& cashAccounts,
        const std::vector<drafts::AccountDraft>& securityAccounts,
        const Set<std::string>&                  tickers,
        QMainWindow*                             mainWindow
    )
        : cash(new DepositWithdrawalWidget(
              TransactionType::Deposit,   // dummy type
              cashAccounts,
              mainWindow
          )),
          stock(new StockWidget(
              securityAccounts,
              cashAccounts,
              tickers,
              mainWindow
          )),
          option(new OptionWidget(
              securityAccounts,
              cashAccounts,
              tickers,
              mainWindow
          ))
    {
    }

    /**
     * @brief Construct a new Transaction Side Bar Controller::
     * Transaction Side Bar Controller object
     *
     * @param undoStack The undo stack for the application
     * @param accountStore The account store for the application
     * @param transactionStore The transaction store for the application
     * @param stockStore The stock store for the application
     * @param optionStore The option store for the application
     * @param positionStore The position store for the application
     * @param transactionController The transaction controller for the
     * application
     * @param stockController The stock controller for the application
     * @param mainWindow The main window of the application
     */
    TransactionSideBarController::TransactionSideBarController(
        cmd::UndoStack&                           undoStack,
        const std::shared_ptr<IAccountStore>&     accountStore,
        const std::shared_ptr<ITransactionStore>& transactionStore,
        const std::shared_ptr<IStockStore>&       stockStore,
        const std::shared_ptr<IOptionStore>&      optionStore,
        const std::shared_ptr<IPositionStore>&    positionStore,
        TransactionController&                    transactionController,
        SecuritiesSideBarController&              stockController,
        QMainWindow*                              mainWindow
    )
        : SideBarCategoryController(new TransactionCategory(), mainWindow),
          _undoStack(undoStack),
          _accountStore(accountStore),
          _transactionStore(transactionStore),
          _positionStore(positionStore),
          _stockStore(stockStore),
          _optionStore(optionStore),
          _dialogs(nullptr),
          _transactionController(transactionController),
          _stockController(stockController),
          _connections(std::make_unique<Connections>())
    {
        const auto cashAccounts =
            AccountMapper::toDrafts(_accountStore->getCashAccounts());

        const auto securityAccounts =
            AccountMapper::toDrafts(_accountStore->getSecurityAccounts());

        _dialogs = std::make_unique<Dialogs>(
            cashAccounts,
            securityAccounts,
            _stockStore->getAllTickers(),
            mainWindow
        );

        connect(
            _dialogs->cash,
            &DepositWithdrawalWidget::createCashTransactionRequested,
            this,
            &TransactionSideBarController::_onCreateCashTransactionRequested
        );

        connect(
            _dialogs->stock,
            &StockWidget::createTickerRequested,
            this,
            &TransactionSideBarController::_onCreateTickerRequested
        );

        connect(
            _dialogs->stock,
            &StockWidget::createStockTransactionRequested,
            this,
            &TransactionSideBarController::_onCreateStockTransactionRequested
        );

        connect(
            _dialogs->option,
            &OptionWidget::createOptionTransactionRequested,
            this,
            &TransactionSideBarController::_onCreateOptionTransactionRequested
        );

        connect(
            _dialogs->option,
            &OptionWidget::createTickerRequested,
            this,
            &TransactionSideBarController::_onCreateTickerRequested
        );

        _connections->add(_stockStore->subscribeToStoreChange(
            [&]()
            {
                _dialogs->stock->updateTickers(_stockStore->getAllTickers());
                _dialogs->option->updateTickers(_stockStore->getAllTickers());
            },
            this
        ));
    }

    TransactionSideBarController::~TransactionSideBarController() = default;

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

            _dialogs->cash->setTransactionType(type);
            _dialogs->cash->updateAccounts(
                AccountMapper::toDrafts(_accountStore->getCashAccounts())
            );
            _dialogs->cash->refresh();

            _dialogs->cash->show();
        }
        else if (action == item->getCreateStockTransactionAction())
        {
            _dialogs->stock->updateAccounts(
                AccountMapper::toDrafts(_accountStore->getSecurityAccounts())
            );
            _dialogs->stock->updateReferenceAccounts(
                AccountMapper::toDrafts(_accountStore->getCashAccounts())
            );
            _dialogs->stock->updateTickers(_stockStore->getAllTickers());
            _dialogs->stock->refresh();

            _dialogs->stock->show();
        }
        else if (action == item->getCreateOptionTransactionAction())
        {
            _dialogs->option->updateAccounts(
                AccountMapper::toDrafts(_accountStore->getSecurityAccounts())
            );
            _dialogs->option->updateReferenceAccounts(
                AccountMapper::toDrafts(_accountStore->getCashAccounts())
            );
            _dialogs->option->updateTickers(_stockStore->getAllTickers());
            _dialogs->option->refresh();

            _dialogs->option->show();
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
        const drafts::CreateCashTransactionDraft& draft
    )
    {
        LOG_ENTRY;

        const auto transaction =
            TransactionCreateMapper::fromCreateCashDraft(draft);

        const auto result = _transactionStore->addCashTransaction(transaction);

        if (!_checkAddTransaction(result))
            return;

        // TODO(97gamjak): add here commands
        _dialogs->cash->close();

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

        auto drafts = getOpenStockPositions(
            draft.getSecurityAccount(),
            _positionStore,
            _stockStore,
            _transactionStore
        );

        // TODO(97gamjak): as soon as coordinators are available this will be
        // handled by them
        // TODO(97gamjak): add also check for which account was requested
        // remove from drafts if ticker does not match
        std::erase_if(
            drafts,
            [&draft](const drafts::PositionDraft& positionDraft)
            {
                return draft.getTicker() !=
                       positionDraft.getStockInfo().getTicker();
            }
        );

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
            positionId    = _positionStore->createPosition(position);
        }

        draft.setPositionId(positionId);

        const auto transaction =
            TransactionCreateMapper::fromCreateStockDraft(draft);

        const auto txAddResult =
            _transactionStore->addStockTransaction(transaction);

        if (!_checkAddTransaction(txAddResult))
            return;

        // TODO(97gamjak): add here commands and also error handling
        _dialogs->stock->close();
        _transactionController.transactionOverviewSelected(false);
    }

    /**
     * @brief Handle the creation of a new option transaction, this will be
     * called when the user submits the create transaction dialog for an option
     * transaction, and should handle validating the transaction draft, adding
     * any necessary additional entries (e.g. for external accounts), and then
     * adding the transaction to the store. This allows the controller to manage
     * the process of creating a new option transaction from the UI, ensuring
     * that the transaction is properly validated and added to the store with
     * all necessary information.
     *
     * @param draft The draft of the option transaction to create, this contains
     * all the necessary information for creating a new option transaction,
     * including the timestamp, legs, and any optional comment.
     */
    void TransactionSideBarController::_onCreateOptionTransactionRequested(
        drafts::CreateOptionTransactionDraft draft
    )
    {
        LOG_ENTRY;

        const auto result = convertTickerToInstrumentId(draft, _stockStore);

        if (!result)
            throw std::logic_error(result.error());

        const auto stock =
            _stockStore->getStock(draft.getUnderlyingInstrumentId());

        if (!stock)
        {
            const auto msg =
                "Failed to retrieve stock data for underlying "
                "instrument with ID " +
                draft.getUnderlyingInstrumentId().toString();

            LOG_ERROR(msg);
            throw std::logic_error(msg);
        }

        const auto option = OptionMapper::toOption(draft, stock.value());

        const auto optionResult = _optionStore->addOption(option);

        if (!optionResult)
        {
            const auto msg =
                "Failed to add option: " +
                std::to_string(static_cast<int>(optionResult.error())) + ". " +
                GithubConstants::getCreateIssueError();

            LOG_ERROR(msg);
            throw std::logic_error(msg);
        }

        draft.setInstrumentId(optionResult.value());

        auto       position   = Position(draft.getTimestamp());
        const auto positionId = _positionStore->createPosition(position);

        draft.setPositionId(positionId);

        const auto transaction =
            TransactionCreateMapper::fromCreateOptionDraft(draft);

        const auto txAddResult =
            _transactionStore->addOptionTransaction(transaction);

        if (!_checkAddTransaction(txAddResult))
            return;

        // TODO(97gamjak): add here commands and also error handling
        _dialogs->option->close();
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
     * @brief Check the result of adding a transaction to the store
     *
     * @param result The result of the transaction store add operation
     * @return true if the transaction was added successfully, false otherwise
     */
    bool TransactionSideBarController::_checkAddTransaction(
        store::TransactionStoreResult result
    )
    {
        switch (result)
        {
            case TransactionStoreResult::Ok:
                return true;

            case TransactionStoreResult::Error:
            case TransactionStoreResult::TransactionSumNotZero:
            {
                const auto msg = "Failed to create cash transaction: " +
                                 TransactionStoreResultMeta::toString(result) +
                                 ". " + GithubConstants::getCreateIssueError();

                LOG_ERROR(msg);
                ErrorDialog::show(msg);
                return false;
            }
        }

        std::unreachable();
    }

}   // namespace controller

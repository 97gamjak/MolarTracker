#include "transaction_controller.hpp"

#include <stdexcept>

#include "app/store/account_store.hpp"
#include "app/store/transaction_store.hpp"
#include "config/finance.hpp"
#include "drafts/transaction_draft.hpp"
#include "drafts/transaction_mapper.hpp"
#include "logging/log_macros.hpp"
#include "ui/side_bar/transaction_category.hpp"
#include "ui/transaction/create_transaction_dlg.hpp"

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
        cmd::UndoStack&        undoStack,
        app::AccountStore&     accountStore,
        app::TransactionStore& transactionStore,
        TransactionController& transactionController,
        QMainWindow*           mainWindow
    )
        : SideBarCategoryController(new ui::TransactionCategory(), mainWindow),
          _undoStack(undoStack),
          _accountStore(accountStore),
          _transactionStore(transactionStore),
          _createDlg(new ui::CreateTransactionDialog(mainWindow)),
          _transactionController(transactionController)
    {
        connect(
            _createDlg,
            &ui::CreateTransactionDialog::transactionTypeChanged,
            this,
            &TransactionSideBarController::_onTransactionTypeChanged
        );

        connect(
            _createDlg,
            &ui::CreateTransactionDialog::createCashTransactionRequested,
            this,
            &TransactionSideBarController::_onCreateCashTransactionRequested
        );
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
            action == item->getCreateWithdrawalAction() ||
            action == item->getCreateStockTransactionAction())
        {
            const auto type = action->data().value<TransactionType>();

            LOG_DEBUG(
                "Create action triggered for transaction category with type: " +
                TransactionTypeMeta::toString(type)
            );

            _onTransactionTypeChanged(type);

            if (auto* dialog = _createDlg.data())
                dialog->exec();
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
     * @brief Handle the selection of transactions in the side bar, this will
     * trigger the transaction overview to update with the latest data from the
     * store, and provides a way for the UI to trigger updates to the
     * transaction overview when it is selected.
     *
     * @param type The type of transactions to show in the overview, this allows
     * the controller to filter the transactions displayed based on their type.
     */
    void TransactionSideBarController::_onTransactionTypeChanged(
        TransactionType type
    )
    {
        LOG_DEBUG(
            std::format(
                "Transaction type changed to '{}'",
                TransactionTypeMeta::toString(type)
            )
        );

        if (_createDlg != nullptr)
        {
            std::vector<drafts::AccountDraft> accounts;
            std::vector<drafts::AccountDraft> referenceAccounts;

            switch (type)
            {
                case TransactionType::Deposit:
                case TransactionType::Withdrawal:
                    accounts = _accountStore.getCashAccounts();
                    // no need to set reference accounts for withdrawal or
                    // Deposit here we use external accounts which are
                    // determined automatically
                    break;
                case TransactionType::Stock:
                    accounts          = _accountStore.getSecurityAccounts();
                    referenceAccounts = _accountStore.getCashAccounts();
                    break;
            }

            _createDlg->setWidget(type, accounts, referenceAccounts);
        }
    }

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
        _createDlg->close();
        _createDlg->reset();
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

}   // namespace controller

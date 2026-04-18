#include "transaction_controller.hpp"

#include "app/store/account_store.hpp"
#include "app/store/transaction_store.hpp"
#include "config/finance.hpp"
#include "drafts/transaction_draft.hpp"
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
            &ui::CreateTransactionDialog::createTransactionRequested,
            this,
            &TransactionSideBarController::_onCreateTransactionRequested
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

        if (action == item->getCreateAction() ||
            action == item->getCreateDepositAction() ||
            action == item->getCreateWithdrawalAction())
        {
            std::optional<TransactionType> type;

            if (action == item->getCreateDepositAction())
                type = TransactionType::Deposit;
            else if (action == item->getCreateWithdrawalAction())
                type = TransactionType::Withdrawal;
            else
                type = std::nullopt;

            LOG_DEBUG(
                "Create action triggered for transaction category with type: " +
                (type.has_value() ? TransactionTypeMeta::toString(type.value())
                                  : "None")
            );

            if (type.has_value())
                _onTransactionTypeChanged(type.value());

            if (auto* dialog = _createDlg.data())
                dialog->exec();
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

            switch (type)
            {
                case TransactionType::Deposit:
                case TransactionType::Withdrawal:
                    accounts = _accountStore.getCashAccounts();
                    break;
            }

            _createDlg->setWidget(type, accounts);
        }
    }

    /**
     * @brief Handle the creation of a new transaction, this will take the
     * transaction draft from the create transaction dialog, convert it to a
     * transaction model, and add it to the transaction store, this allows the
     * controller to handle the creation of new transactions based on the input
     * from the user in the create transaction dialog, and ensures that new
     * transactions are properly added to the store and can be displayed in the
     * UI.
     *
     * @param draft The transaction draft containing the information needed to
     * create a new transaction, this includes details such as the type of
     * transaction, the entries for the transaction, and any other relevant
     * information needed to create a new transaction in the store.
     */
    void TransactionSideBarController::_onCreateTransactionRequested(
        drafts::TransactionDraft draft
    )
    {
        std::vector<drafts::TransactionEntryDraft> additionalEntries;

        for (auto entry : draft.entries)
        {
            if (entry.needsExternal)
            {
                entry.accountId =
                    _accountStore.getExternalAccount(entry.cash.getCurrency());
                entry.cash = -entry.cash;
                additionalEntries.push_back(entry);
            }
        }

        for (const auto& entry : additionalEntries)
        {
            draft.entries.push_back(entry);
        }

        _transactionStore.addTransaction(draft);
        // TODO(97gamjak): add here commands and also error handling
        _createDlg->close();
        _transactionController.transactionOverviewSelected(false);
    }

    /**
     * @brief Handle the selection of transactions in the side bar, this will
     * trigger the transaction overview to update with the latest data from the
     * store, and provides a way for the UI to trigger updates to the
     * transaction overview when it is selected.
     *
     */
    void TransactionSideBarController::onTransactionsSelected()
    {
        _transactionController.transactionOverviewSelected();
    }

}   // namespace controller

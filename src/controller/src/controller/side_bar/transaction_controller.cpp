#include "transaction_controller.hpp"

#include "app/store/account_store.hpp"
#include "config/finance.hpp"
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
     * @param mainWindow The main window of the application
     */
    TransactionSideBarController::TransactionSideBarController(
        cmd::UndoStack&    undoStack,
        app::AccountStore& accountStore,
        QMainWindow*       mainWindow
    )
        : SideBarCategoryController(new ui::TransactionCategory(), mainWindow),
          _undoStack(undoStack),
          _accountStore(accountStore),
          _createDlg(new ui::CreateTransactionDialog(mainWindow))
    {
        connect(
            _createDlg,
            &ui::CreateTransactionDialog::transactionTypeChanged,
            this,
            &TransactionSideBarController::_onTransactionTypeChanged
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

}   // namespace controller

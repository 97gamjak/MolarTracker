#include "ui/transaction/transactions_overview.hpp"

namespace ui
{

    TransactionsOverview::TransactionsOverview(QWidget* parent)
        : QWidget(parent), _transactionTable(new TransactionTable(this))
    {
        // build ui to show lines of transactions as a table
    }

    void TransactionsOverview::refresh(
        const std::vector<drafts::TransactionDraft>& transactions
    )
    {
        _transactionTable->getModel()->setTransactions(transactions);
    }
}   // namespace ui
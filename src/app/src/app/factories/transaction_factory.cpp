#include "transaction_factory.hpp"

#include "config/finance_enums.hpp"
#include "finance/transaction_entry.hpp"
#include "sql_models/instrument_row.hpp"
#include "sql_models/transaction_row.hpp"

namespace app
{
    TransactionRow TransactionFactory::toTransactionRow(
        const finance::Transaction &transaction
    )
    {
        TransactionRow row;

        row.id        = transaction.getId();
        row.timestamp = transaction.getTimestamp();
        row.status    = transaction.getStatus();
        row.comment   = transaction.getComment();

        return row;
    }

    finance::Transaction TransactionFactory::fromTransactionRow(
        const TransactionRow &row
    )
    {
        finance::Transaction transaction{
            row.id.value(),
            row.timestamp.value(),
            row.status.value(),
            row.comment.value()
        };
        return transaction;
    }

    TransactionEntryRow TransactionFactory::toTransactionEntryRow(
        const finance::TransactionEntry &entry,
        TransactionId                    transactionId,
        InstrumentId                     instrumentId
    )
    {
        TransactionEntryRow row;

        row.id            = entry.getId();
        row.transactionId = transactionId;
        row.instrumentId  = instrumentId;
        row.accountId     = entry.getAccountId();

        row.amount = std::visit(finance::AmountVisitor{}, entry.getDetails());

        return row;
    }

    InstrumentRow TransactionFactory::toInstrumentRow(
        const finance::TransactionDetail &detail
    )
    {
        struct InstrumentRowVisitor
        {
            InstrumentRow operator()(const finance::CashTransaction
                                         & /*cash*/) const
            {
                InstrumentRow row;
                row.kind = InstrumentKind::Cash;
                return row;
            }

            // Add more visit methods for other transaction types as needed
        };

        return std::visit(InstrumentRowVisitor{}, detail);
    }

}   // namespace app
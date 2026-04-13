#include "transaction_factory.hpp"

#include "config/finance.hpp"
#include "finance/transaction_entry.hpp"
#include "sql_models/instrument_row.hpp"
#include "sql_models/transaction_row.hpp"

namespace app
{
    /**
     * @brief Converts a Transaction object to a TransactionRow object.
     *
     * @param transaction The Transaction object to convert.
     * @return The converted TransactionRow object.
     */
    TransactionRow TransactionFactory::toRow(
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

    /**
     * @brief Converts a TransactionRow object to a Transaction object.
     *
     * @param row The TransactionRow object to convert.
     * @return The converted Transaction object.
     */
    finance::Transaction TransactionFactory::fromRow(const TransactionRow &row)
    {
        finance::Transaction transaction{
            row.id.value(),
            row.timestamp.value(),
            row.status.value(),
            row.comment.value()
        };
        return transaction;
    }

    // finance::TransactionEntry TransactionFactory::fromTransactionEntryRow(
    //     const TransactionEntryRow &entryRow
    // )
    // {
    //     switch (entryRow.instrumentId.value())
    //     {
    //         case InstrumentKind::Cash:
    //             detail = finance::CashTransaction{entryRow.amount.value()};
    //             break;
    //     }

    //     return finance::TransactionEntry{
    //         entryRow.id.value(),
    //         entryRow.accountId.value(),
    //         detail
    //     };
    // }

    // finance::CashTransaction TransactionFactory::fromInstrumentRow(
    //     const InstrumentRow &row
    // )
    // {
    //     switch (row.kind.value())
    //     {
    //         case InstrumentKind::Cash:
    //             return finance::CashTransaction{};
    //     }

    //     throw std::runtime_error("Unknown instrument kind");
    // }

    /**
     * @brief Converts a TransactionEntry object to a
     * TransactionEntryRow object.
     *
     * @param entry The TransactionEntry object to convert.
     * @param transactionId The ID of the associated transaction.
     * @param instrumentId The ID of the associated instrument.
     * @return The converted TransactionEntryRow object.
     */
    TransactionEntryRow TransactionFactory::toEntryRow(
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

    finance::TransactionEntry TransactionFactory::fromEntryRow(
        const TransactionEntryRow &row,
        const InstrumentRow       &instrumentRow
    )
    {
        finance::TransactionEntry entry{
            row.id.value(),
            row.accountId.value(),
            finance::AmountVisitor::fromRow(row.amount)
        };

        entry.setInstrument(instrumentRow);

        return entry;
    }

    /**
     * @brief Converts a TransactionDetail object to an
     * InstrumentRow object.
     *
     * @param detail The TransactionDetail object to convert.
     * @return The converted InstrumentRow object.
     */
    InstrumentRow TransactionFactory::toInstrumentRow(
        const finance::TransactionDetail &detail
    )
    {
        /**
         * @brief Visitor for converting TransactionDetail to
         * InstrumentRow.
         *
         */
        struct InstrumentRowVisitor
        {
            InstrumentRow operator()(const finance::CashTransaction
                                         & /*cash*/) const
            {
                InstrumentRow row;
                row.kind = InstrumentKind::Cash;
                return row;
            }
        };

        return std::visit(InstrumentRowVisitor{}, detail);
    }

}   // namespace app
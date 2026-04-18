#include "transaction_factory.hpp"

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "finance/transaction_entry.hpp"
#include "sql_models/instrument_row.hpp"
#include "sql_models/transaction_entry_row.hpp"
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

        row.amount = entry.getAmount();

        return row;
    }

    /**
     * @brief Converts a TransactionEntryRow object to a TransactionEntry
     * object.
     *
     * @param row The TransactionEntryRow object to convert.
     * @param instrumentRow The associated InstrumentRow object.
     * @return The converted TransactionEntry object.
     */
    finance::TransactionEntry TransactionFactory::fromEntryRow(
        const TransactionEntryRow &row,
        const InstrumentRow       &instrumentRow
    )
    {
        switch (instrumentRow.kind.value())
        {
            case InstrumentKind::Cash:
                return {
                    row.id.value(),
                    row.accountId.value(),
                    _toCashTransaction(row, instrumentRow)
                };
            case InstrumentKind::Stock:
                throw std::runtime_error(
                    "Stock transactions are not supported"
                );
        }

        throw std::runtime_error("Unknown instrument kind");
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

    /**
     * @brief Converts a TransactionEntryRow object and an InstrumentRow
     * object to a CashTransaction object.
     *
     * @param row The TransactionEntryRow object to convert.
     * @param instrumentRow The associated InstrumentRow object.
     * @return The converted CashTransaction object.
     */
    finance::CashTransaction TransactionFactory::_toCashTransaction(
        const TransactionEntryRow &row,
        const InstrumentRow       &instrumentRow
    )
    {
        if (instrumentRow.kind.value() != InstrumentKind::Cash)
        {
            throw std::runtime_error("Invalid instrument kind");
        }

        return finance::CashTransaction(
            finance::Cash(instrumentRow.currency.value(), row.amount.value())
        );
    }
}   // namespace app
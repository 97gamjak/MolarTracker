#include "transaction_factory.hpp"

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "finance/transaction.hpp"
#include "finance/transaction_entry.hpp"
#include "finance/transaction_filter.hpp"
#include "sql_models/trade_leg_row.hpp"
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
        row.type      = transaction.getType();

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
        finance::TransactionData type;

        switch (row.type.value())
        {
            case TransactionDataType::Cash:
                type = finance::CashData{};
                break;
            case TransactionDataType::Trade:
                type = finance::TradeData{};
                break;
        }

        finance::Transaction transaction{
            row.id.value(),
            row.timestamp.value(),
            row.status.value(),
            type,
            {},
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
     * @return The converted TransactionEntryRow object.
     */
    TransactionEntryRow TransactionFactory::toEntryRow(
        const finance::TransactionEntry &entry,
        TransactionId                    transactionId
    )
    {
        TransactionEntryRow row;

        row.id            = entry.getId();
        row.transactionId = transactionId;
        row.accountId     = entry.getAccountId();

        row.amount   = entry.getAmount();
        row.currency = entry.getCurrency();

        return row;
    }

    /**
     * @brief Converts a TransactionEntryRow object to a TransactionEntry
     * object.
     *
     * @param row The TransactionEntryRow object to convert.
     * @return The converted TransactionEntry object.
     */
    finance::TransactionEntry TransactionFactory::fromEntryRow(
        const TransactionEntryRow &row
    )
    {
        return {
            row.id.value(),
            row.accountId.value(),
            finance::Cash(row.currency.value(), row.amount.value())
        };
    }

    /**
     * @brief Converts a TradeLeg object to a TradeLegRow object.
     *
     * @param leg The TradeLeg object to convert.
     * @param transactionId The ID of the associated transaction.
     * @return The converted TradeLegRow object.
     */
    TradeLegRow TransactionFactory::toLegRow(
        const finance::TradeLeg &leg,
        TransactionId            transactionId
    )
    {
        TradeLegRow row;

        row.id            = TradeLegId::invalid();
        row.transactionId = transactionId;
        row.instrumentId  = leg.getInstrumentId();
        row.accountId     = leg.getAccountId();
        row.quantity      = leg.getQuantity().toMicroUnits();
        row.unitPrice     = leg.getUnitPrice().getAmount();
        row.currency      = leg.getUnitPrice().getCurrency();
        row.positionId    = leg.getPositionId();

        return row;
    }

    /**
     * @brief Converts a TradeLegRow object to a TradeLeg object.
     *
     * @param row The TradeLegRow object to convert.
     * @return The converted TradeLeg object.
     */
    finance::TradeLeg TransactionFactory::fromLegRow(const TradeLegRow &row)
    {
        return {
            row.accountId.value(),
            row.instrumentId.value(),
            Quantity(row.quantity.value()),
            finance::Cash(row.currency.value(), row.unitPrice.value()),
            row.positionId.value()
        };
    }

    orm::WhereExpr TransactionFactory::toWhereExpr(
        const finance::TransactionFilter &filter
    )
    {
        orm::WhereExpr where;

        if (filter.getPositionId().has_value())
        {
            where &= TradeLegRow::hasPosition(filter.getPositionId().value());
        }

        return where;
    }

}   // namespace app
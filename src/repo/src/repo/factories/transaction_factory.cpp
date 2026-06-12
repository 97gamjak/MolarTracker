#include "transaction_factory.hpp"

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/transaction_entry.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "orm/where_expr.hpp"
#include "sql_models/trade_leg_row.hpp"
#include "sql_models/transaction_entry_row.hpp"
#include "sql_models/transaction_option_row.hpp"
#include "sql_models/transaction_row.hpp"

namespace repo
{
    /**
     * @brief Converts a Transaction object to a TransactionRow object.
     *
     * @param transaction The Transaction object to convert.
     * @return The converted TransactionRow object.
     */
    TransactionRow TransactionFactory::toRow(
        const finance::DomainTransaction &transaction
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
     * @param optionRow An optional TransactionOptionRow object to include in
     * the conversion, this is used to provide additional details for option
     * transactions, allowing for a more complete representation of
     * option-related transactions when converting from a TransactionRow to a
     * Transaction object.
     * @return The converted Transaction object.
     */
    finance::DomainTransaction TransactionFactory::fromRow(
        const TransactionRow                      &row,
        const std::optional<TransactionOptionRow> &optionRow
    )
    {
        finance::TransactionData type;

        switch (row.type.value())
        {
            case TransactionDataType::Cash:
                type = finance::CashData{};
                break;
            case TransactionDataType::Stock:
                type = finance::TradeData{};
                break;
            case TransactionDataType::Option:
                type = finance::OptionData{
                    optionRow->id.value(),
                    optionRow->buySell.value(),
                    optionRow->action.value(),
                    optionRow->rolledOption.value()
                };
                break;
        }

        finance::DomainTransaction transaction{
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
        row.type     = entry.getType();

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
            finance::Cash(row.currency.value(), row.amount.value()),
            row.type.value()
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

    /**
     * @brief Converts an OptionData object to a TransactionOptionRow object.
     *
     * @param optionData The OptionData object to convert.
     * @param transactionId The ID of the associated transaction.
     * @return The converted TransactionOptionRow object.
     */
    TransactionOptionRow TransactionFactory::toOptionRow(
        const finance::OptionData &optionData,
        TransactionId              transactionId
    )
    {
        TransactionOptionRow row;

        row.id            = optionData.getId();
        row.transactionId = transactionId;
        row.buySell       = optionData.getBuySell();
        row.action        = optionData.getAction();
        row.rolledOption  = optionData.getRolledOption();

        return row;
    }

    /**
     * @brief Convert a TransactionFilter to a WhereExpr for querying the
     * database, this factory method takes a TransactionFilter object as input
     * and creates a corresponding WhereExpr that can be used to query the
     * database for transactions that match the criteria specified in the
     * filter, ensuring that the filtering logic is correctly translated into a
     * format that can be executed by the database.
     *
     * @param filter The TransactionFilter to convert.
     * @return orm::WhereExpr The resulting WhereExpr for querying the database.
     */
    orm::WhereExpr TransactionFactory::toWhereExpr(
        const finance::TransactionFilter &filter
    )
    {
        orm::WhereExpr where = orm::makeEmptyWhere();

        if (!filter.getTransactionIds().empty())
        {
            orm::WhereExpr idWhere = orm::makeEmptyWhere();

            for (const auto &transactionId : filter.getTransactionIds())
            {
                idWhere |= TransactionRow::hasTransactionId(transactionId);
            }

            where &= idWhere;
        }

        if (!filter.getPositionIds().empty())
        {
            orm::WhereExpr posIdWhere = orm::makeEmptyWhere();

            for (const auto &positionId : filter.getPositionIds())
            {
                posIdWhere |= TradeLegRow::hasPosition(positionId);
            }

            where &= posIdWhere;
        }

        return where;
    }

    /** @brief Converts a TransactionFilter object to a Join object for
     * querying the database, this factory method takes a TransactionFilter
     * object as input and creates a corresponding Join object that can be
     * used to query the database for transactions that match the criteria
     * specified in the filter, ensuring that the joining logic is correctly
     * translated into a format that can be executed by the database.
     *
     * @param filter The TransactionFilter to convert.
     * @return orm::Joins The resulting Join object for querying the database.
     */
    orm::Joins TransactionFactory::toJoin(
        const finance::TransactionFilter &filter
    )
    {
        orm::Joins join;

        if (!filter.getPositionIds().empty())
        {
            join = join.add(
                orm::join<
                    TransactionRow::idField,
                    TradeLegRow::transactionIdField>()
            );
        }

        return join;
    }

}   // namespace repo
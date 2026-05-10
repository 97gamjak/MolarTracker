#include "sql_models/trade_leg_row.hpp"

#include "filter/operators.hpp"

/**
 * @brief Create a where expression to filter trade legs by transaction ID
 *
 * @param transactionId The transaction ID to filter by
 * @return orm::WhereExpr The where expression
 */
orm::WhereExpr TradeLegRow::hasTransactionId(TransactionId transactionId)
{
    return orm::makeWhere(
        transactionIdField(transactionId),
        filter::Operator::Equal
    );
}

orm::WhereExpr TradeLegRow::hasPosition(PositionId positionId)
{
    return orm::makeWhere(positionIdField(positionId), filter::Operator::Equal);
}
#include "sql_models/trade_leg_row.hpp"

#include "filter/operators.hpp"

orm::WhereExpr TradeLegRow::hasTransactionId(TransactionId transactionId)
{
    return orm::makeWhere(
        transactionIdField(transactionId),
        filter::Operator::Equal
    );
}
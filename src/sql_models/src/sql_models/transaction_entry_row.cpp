#include "sql_models/transaction_entry_row.hpp"

#include "orm/where_expr.hpp"

/**
 * @brief Get a WhereExpr for filtering transaction entries by transaction ID
 *
 * @param transactionId
 * @return orm::WhereExpr
 */
orm::WhereExpr TransactionEntryRow::hasTransactionId(
    TransactionId transactionId
)
{
    return orm::makeWhere(
        transactionIdField{transactionId},
        filter::Operator::Equal
    );
}

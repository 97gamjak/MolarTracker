#include "sql_models/transaction_row.hpp"

#include "orm/where_expr.hpp"

/**
 * @brief Creates a WhereExpr to filter transactions by their TransactionId.
 *
 * @param transactionId The TransactionId to filter by.
 * @return orm::WhereExpr A WhereExpr that can be used to filter transactions
 * by the specified TransactionId.
 */
orm::WhereExpr TransactionRow::hasTransactionId(TransactionId transactionId)
{
    return orm::makeWhere<idField>(transactionId, filter::Operator::Equal);
}
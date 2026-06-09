#include "sql_models/transaction_row.hpp"

#include "orm/where_expr.hpp"

orm::WhereExpr TransactionRow::hasTransactionId(TransactionId transactionId)
{
    return orm::makeWhere<idField>(transactionId, filter::Operator::Equal);
}
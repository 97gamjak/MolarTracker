#include "sql_models/transaction_option_row.hpp"

#include "orm/where_expr.hpp"

/**
 * @brief Generates a WhereExpr to filter TransactionOptionRow by transactionId,
 * this method creates a WhereExpr that can be used to query the database for
 * TransactionOptionRow entries that are associated with a specific transaction
 * ID, allowing for efficient retrieval of option data related to a particular
 * transaction, which is essential for analyzing and managing options within
 * financial transactions.
 *
 * @param transactionId The ID of the transaction to filter by.
 * @return orm::WhereExpr
 */
orm::WhereExpr TransactionOptionRow::hasTransactionId(
    TransactionId transactionId
)
{
    return orm::makeWhere<TransactionOptionRow::transactionIdField>(
        transactionId,
        filter::Operator::Equal
    );
}
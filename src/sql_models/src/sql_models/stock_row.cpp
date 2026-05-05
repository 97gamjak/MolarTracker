#include "sql_models/stock_row.hpp"

#include "orm/where_expr.hpp"

/**
 * @brief Create a WhereExpr to filter StockRows by ticker
 *
 * @param ticker The ticker symbol to filter by
 * @return orm::WhereExpr
 */
orm::WhereExpr StockRow::hasTicker(const std::string& ticker)
{
    return orm::makeWhere(tickerField{ticker}, filter::Operator::Equal);
}
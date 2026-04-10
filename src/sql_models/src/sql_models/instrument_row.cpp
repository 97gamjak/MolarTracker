#include "sql_models/instrument_row.hpp"

#include "orm/where_expr.hpp"

/**
 * @brief Get a WhereExpr for filtering instruments by kind
 *
 * @param kind
 * @return orm::WhereExpr
 */
orm::WhereExpr InstrumentRow::hasKind(InstrumentKind kind)
{
    return orm::makeWhere(kindField{kind}, filter::Operator::Equal);
}
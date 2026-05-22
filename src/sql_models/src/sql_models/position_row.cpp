#include "sql_models/position_row.hpp"

#include "orm/where_expr.hpp"

/**
 * @brief Check if the position is open
 *
 * @return orm::WhereExpr
 */
orm::WhereExpr PositionRow::IsOpen()
{
    return orm::makeIsNull<closedAtField>();
}
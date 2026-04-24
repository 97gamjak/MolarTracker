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

InstrumentRow InstrumentRow::createCashInstrument(Currency currency)
{
    InstrumentRow row;
    row.kind     = InstrumentKind::Cash;
    row.currency = currency;
    return row;
}

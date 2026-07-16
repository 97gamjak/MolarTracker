#include "sql_models/option_row.hpp"

#include "config/id_types.hpp"
#include "filter/operators.hpp"
#include "orm/where_expr.hpp"
#include "utils/finance.hpp"
#include "utils/quantity.hpp"

/**
 * @brief Creates a where expression to find an option by its underlying
 * instrument ID, option type, strike price, and expiration date.
 *
 * @param underlying
 * @param optionType
 * @param strikePrice
 * @param expirationDate
 * @return orm::WhereExpr
 */
orm::WhereExpr OptionRow::hasName(
    InstrumentId underlying,
    OptionType   optionType,
    micro_units  strikePrice,
    Timestamp    expirationDate
)
{
    auto where = orm::WhereExpr{};
    where      = where && orm::makeWhere<underlyingInstrumentIdField>(
                         underlying,
                         filter::Operator::Equal
                     );
    where =
        where &&
        orm::makeWhere<optionTypeField>(optionType, filter::Operator::Equal);

    where =
        where &&
        orm::makeWhere<strikePriceField>(strikePrice, filter::Operator::Equal);

    where = where && orm::makeWhere<expirationDateField>(
                         expirationDate,
                         filter::Operator::Equal
                     );
    return where;
}
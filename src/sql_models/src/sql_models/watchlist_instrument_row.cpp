#include "sql_models/watchlist_instrument_row.hpp"

/**
 * @brief Get a WhereExpr for filtering watchlist instruments by watchlist ID
 *
 * @param watchlistId
 * @return orm::WhereExpr
 */
orm::WhereExpr WatchlistInstrumentRow::hasWatchlistId(
    const WatchlistId& watchlistId
)
{
    return orm::makeWhere<watchlistIdField>(
        watchlistId,
        filter::Operator::Equal
    );
}

/**
 * @brief Get a WhereExpr for filtering watchlist instruments by watchlist ID
 * and symbol
 *
 * @param watchlistId
 * @param symbol
 * @return orm::WhereExpr
 */
orm::WhereExpr WatchlistInstrumentRow::hasWatchlistIdAndSymbol(
    const WatchlistId& watchlistId,
    const std::string& symbol
)
{
    return hasWatchlistId(watchlistId) &&
           orm::makeWhere<symbolField>(symbol, filter::Operator::Equal);
}

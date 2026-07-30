#include "sql_models/watchlist_row.hpp"

/**
 * @brief Construct a new Watchlist Row:: Watchlist Row object
 *
 * @param _id
 */
WatchlistRow::WatchlistRow(WatchlistId _id) : id{_id} {}

/**
 * @brief Get a WhereExpr for filtering watchlists by name
 *
 * @param name
 * @return orm::WhereExpr
 */
orm::WhereExpr WatchlistRow::hasName(const std::string& name)
{
    return orm::makeWhere<nameField>(name, filter::Operator::Equal);
}

/**
 * @brief Get a WhereExpr for filtering watchlists by id
 *
 * @param id
 * @return orm::WhereExpr
 */
orm::WhereExpr WatchlistRow::hasId(const WatchlistId& id)
{
    return orm::makeWhere<idField>(id, filter::Operator::Equal);
}

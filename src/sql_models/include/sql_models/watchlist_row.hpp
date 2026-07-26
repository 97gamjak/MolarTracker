#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__WATCHLIST_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__WATCHLIST_ROW_HPP__

#include <string>

#include "common/timestamp.hpp"
#include "config/id_types.hpp"
#include "orm/field.hpp"
#include "orm/orm_model.hpp"
#include "orm/where_expr.hpp"

/**
 * @brief Represents a row in the "watchlists" database table
 *
 */
struct WatchlistRow : public orm::ORMModel<"watchlists">
{
    WatchlistRow() = default;
    explicit WatchlistRow(WatchlistId _id);

    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    ORM_FIELD(id, IdField<WatchlistId>)

    /// The name field, this is a required field
    ORM_FIELD(name, Field<"name", std::string, orm::not_null_t>)

    /// The creation timestamp of the watchlist
    ORM_FIELD(createdAt, Field<"created_at", Timestamp, orm::not_null_t>)

    /// @cond DOXYGEN_IGNORE
    ORM_FIELDS(WatchlistRow, id, name, createdAt)
    /// @endcond

    [[nodiscard]] static orm::WhereExpr hasName(const std::string& name);

    [[nodiscard]] static orm::WhereExpr hasId(const WatchlistId& id);
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__WATCHLIST_ROW_HPP__

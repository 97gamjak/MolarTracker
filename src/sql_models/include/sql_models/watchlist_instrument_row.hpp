#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__WATCHLIST_INSTRUMENT_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__WATCHLIST_INSTRUMENT_ROW_HPP__

#include <string>

#include "config/id_types.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/orm_model.hpp"
#include "orm/where_expr.hpp"
#include "watchlist_row.hpp"

/**
 * @brief Represents a row in the "watchlist_instruments" database table,
 * mapping a symbol to the watchlist it belongs to. A symbol is stored as a
 * plain string rather than a foreign key to the instrument table, so a
 * watchlist can reference a symbol that doesn't (yet) exist as a tracked
 * instrument.
 *
 */
struct WatchlistInstrumentRow : public orm::ORMModel<"watchlist_instruments">
{
    /// The id field, this is the primary key of the table and is
    /// auto-incremented. Uniqueness of (watchlistId, symbol) is enforced via
    /// getUniqueGroups() below rather than a composite primary key, since the
    /// ORM only supports a single-column primary key per table.
    ORM_FIELD(id, IdField<WatchlistInstrumentId>)

    /// The watchlist this symbol belongs to, deleting the watchlist cascades
    /// to its instruments
    ORM_FIELD(
        watchlistId,
        Field<
            "watchlist_id",
            WatchlistId,
            orm::foreign_key_t<
                orm::CascadeDelete,
                WatchlistRow,
                decltype(WatchlistRow::id)>,
            orm::not_null_t>
    )

    /// The ticker symbol tracked in the watchlist
    ORM_FIELD(symbol, Field<"symbol", std::string, orm::not_null_t>)

    /// @cond DOXYGEN_IGNORE
    ORM_FIELDS(WatchlistInstrumentRow, id, watchlistId, symbol)
    /// @endcond

    /**
     * @brief Get the Unique Groups object
     *
     * @return auto
     */
    static auto getUniqueGroups()
    {
        return orm::unique_set(
            orm::unique_group<
                &WatchlistInstrumentRow::watchlistId,
                &WatchlistInstrumentRow::symbol>()
        );
    }

    [[nodiscard]] static orm::WhereExpr hasWatchlistId(
        const WatchlistId& watchlistId
    );

    [[nodiscard]] static orm::WhereExpr hasWatchlistIdAndSymbol(
        const WatchlistId& watchlistId,
        const std::string& symbol
    );
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__WATCHLIST_INSTRUMENT_ROW_HPP__

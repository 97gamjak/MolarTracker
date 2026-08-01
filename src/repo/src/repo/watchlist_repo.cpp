#include "watchlist_repo.hpp"

#include "factories/watchlist_factory.hpp"
#include "finance/watchlist.hpp"
#include "logging/log_macros.hpp"
#include "orm/crud.hpp"
#include "orm/query_options.hpp"
#include "repo_errors.hpp"
#include "sql_models/watchlist_instrument_row.hpp"
#include "sql_models/watchlist_row.hpp"

REGISTER_LOG_CATEGORY("Repo.WatchlistRepo");

namespace repo
{

    /**
     * @brief Create a new, empty watchlist in the repository
     *
     * @param watchlist The watchlist object containing the name and other
     * details of the new watchlist
     * @return WatchlistId The ID of the newly created watchlist
     */
    WatchlistId WatchlistRepo::createWatchlist(
        const finance::Watchlist& watchlist
    )
    {
        db::Transaction transaction{_getDb()};

        WatchlistRow watchlistRow;
        watchlistRow.name      = watchlist.getName();
        watchlistRow.createdAt = watchlist.getCreatedAt();

        const auto result = _getCrud().insert(_getDb(), watchlistRow);

        if (!result.has_value())
        {
            transaction.rollback();

            const auto msg = getInsertError(
                result.error(),
                "watchlist with name '" + watchlist.getName() + "'"
            );

            LOG_ERROR(msg);
            throw orm::CrudException(msg);
        }

        const auto watchlistId = WatchlistId(result.value());

        for (const auto& symbol : watchlist.getSymbols())
        {
            WatchlistInstrumentRow row;
            row.watchlistId = watchlistId;
            row.symbol      = symbol;

            const auto symbolResult = _getCrud().insert(_getDb(), row);

            if (!result.has_value())
            {
                const auto msg = getInsertError(
                    result.error(),
                    "watchlist instrument for symbol '" + symbol + "'"
                );

                LOG_ERROR(msg);
                throw orm::CrudException(msg);
            }
        }

        transaction.commit();

        return watchlistId;
    }

    /**
     * @brief Get all watchlists, including their symbols
     *
     * @return std::vector<finance::Watchlist>
     */
    std::vector<finance::Watchlist> WatchlistRepo::getAllWatchlists()
    {
        const auto watchlistRows = _getCrud().get<WatchlistRow>(_getDb());

        std::vector<finance::Watchlist> watchlists;
        watchlists.reserve(watchlistRows.size());

        for (const auto& row : watchlistRows)
        {
            const auto symbols = _getSymbols(row.id.value());
            watchlists.push_back(
                WatchlistFactory::toWatchlistDomain(row, symbols)
            );
        }

        return watchlists;
    }

    /**
     * @brief Update an existing watchlist
     *
     * @param watchlist The watchlist object containing the updated details of
     * the watchlist
     */
    void WatchlistRepo::updateWatchlist(const finance::Watchlist& watchlist)
    {
        WatchlistRow watchlistRow;
        watchlistRow.id        = watchlist.getId();
        watchlistRow.name      = watchlist.getName();
        watchlistRow.createdAt = watchlist.getCreatedAt();

        const auto result = _getCrud().update(_getDb(), watchlistRow);

        if (!result.has_value())
        {
            const auto msg = getUpdateError(
                result.error(),
                "watchlist with ID '" + watchlist.getId().toString() + "'"
            );

            LOG_ERROR(msg);
            throw orm::CrudException(msg);
        }
    }

    /**
     * @brief Delete a watchlist and all of its symbol entries (via
     * ON DELETE CASCADE)
     *
     * @param id
     */
    void WatchlistRepo::deleteWatchlist(WatchlistId id)
    {
        _getCrud().deleteByPk(_getDb(), WatchlistRow{id});
    }

    /**
     * @brief Get the symbols belonging to a watchlist
     *
     * @param id
     * @return std::vector<std::string>
     */
    std::vector<std::string> WatchlistRepo::_getSymbols(WatchlistId id)
    {
        auto query =
            orm::Query{}.where(WatchlistInstrumentRow::hasWatchlistId(id));

        const auto rows =
            _getCrud().get<WatchlistInstrumentRow>(_getDb(), query);

        std::vector<std::string> symbols;
        symbols.reserve(rows.size());

        for (const auto& row : rows)
            symbols.push_back(row.symbol.value());

        return symbols;
    }

}   // namespace repo

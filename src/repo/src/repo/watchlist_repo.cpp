#include "watchlist_repo.hpp"

#include "common/timestamp.hpp"
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
     * @param name
     * @return WatchlistId The ID of the newly created watchlist
     */
    WatchlistId WatchlistRepo::createWatchlist(const std::string& name)
    {
        db::Transaction transaction{_getDb()};

        WatchlistRow watchlistRow;
        watchlistRow.name      = name;
        watchlistRow.createdAt = Timestamp{};

        const auto result = _getCrud().insert(_getDb(), watchlistRow);

        if (!result.has_value())
        {
            transaction.rollback();

            const auto msg = getInsertError(
                result.error(),
                "watchlist with name '" + name + "'"
            );

            LOG_ERROR(msg);
            throw orm::CrudException(msg);
        }

        transaction.commit();

        return WatchlistId(result.value());
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
     * @brief Rename an existing watchlist
     *
     * @param id
     * @param newName
     */
    void WatchlistRepo::renameWatchlist(
        WatchlistId        id,
        const std::string& newName
    )
    {
        auto query = orm::Query{}.where(WatchlistRow::hasId(id));

        const auto existing =
            _getCrud().getUnique<WatchlistRow>(_getDb(), query);

        if (!existing.has_value())
        {
            const auto msg = "Cannot rename watchlist: no watchlist with id '" +
                             id.toString() + "' exists";

            LOG_ERROR(msg);
            throw RepositoryException(msg);
        }

        auto watchlistRow = existing.value();
        watchlistRow.name = newName;

        const auto result = _getCrud().update(_getDb(), watchlistRow);

        if (!result.has_value())
        {
            const auto msg =
                "Failed to rename watchlist: " + result.error().getMessage();

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
     * @brief Add a symbol to a watchlist
     *
     * @param id
     * @param symbol
     */
    void WatchlistRepo::addSymbol(WatchlistId id, const std::string& symbol)
    {
        WatchlistInstrumentRow row;
        row.watchlistId = id;
        row.symbol      = symbol;

        const auto result = _getCrud().insert(_getDb(), row);

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

    /**
     * @brief Remove a symbol from a watchlist, this is a no-op if the symbol
     * isn't in the watchlist
     *
     * @param id
     * @param symbol
     */
    void WatchlistRepo::removeSymbol(WatchlistId id, const std::string& symbol)
    {
        auto query = orm::Query{}.where(
            WatchlistInstrumentRow::hasWatchlistIdAndSymbol(id, symbol)
        );

        const auto rows =
            _getCrud().get<WatchlistInstrumentRow>(_getDb(), query);

        for (const auto& row : rows)
            _getCrud().deleteByPk(_getDb(), row);
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

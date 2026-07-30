#include "watchlist_service.hpp"

#include "finance/watchlist.hpp"
#include "repo/i_watchlist_repo.hpp"

namespace service
{

    /**
     * @brief Construct a new Watchlist Service:: Watchlist Service object
     *
     * @param watchlistRepo
     */
    WatchlistService::WatchlistService(
        const std::shared_ptr<repo::IWatchlistRepo>& watchlistRepo
    )
        : _watchlistRepo(watchlistRepo)
    {
    }

    /**
     * @brief Create a new, empty watchlist
     *
     * @param name
     * @return WatchlistId
     */
    WatchlistId WatchlistService::createWatchlist(const std::string& name)
    {
        return _watchlistRepo->createWatchlist(name);
    }

    /**
     * @brief Get all watchlists
     *
     * @return std::vector<finance::Watchlist>
     */
    std::vector<finance::Watchlist> WatchlistService::getAllWatchlists() const
    {
        return _watchlistRepo->getAllWatchlists();
    }

    /**
     * @brief Rename an existing watchlist
     *
     * @param id
     * @param newName
     */
    void WatchlistService::renameWatchlist(
        WatchlistId         id,
        const std::string& newName
    )
    {
        _watchlistRepo->renameWatchlist(id, newName);
    }

    /**
     * @brief Delete a watchlist
     *
     * @param id
     */
    void WatchlistService::deleteWatchlist(WatchlistId id)
    {
        _watchlistRepo->deleteWatchlist(id);
    }

    /**
     * @brief Add a symbol to a watchlist
     *
     * @param id
     * @param symbol
     */
    void WatchlistService::addSymbol(WatchlistId id, const std::string& symbol)
    {
        _watchlistRepo->addSymbol(id, symbol);
    }

    /**
     * @brief Remove a symbol from a watchlist
     *
     * @param id
     * @param symbol
     */
    void WatchlistService::removeSymbol(
        WatchlistId         id,
        const std::string& symbol
    )
    {
        _watchlistRepo->removeSymbol(id, symbol);
    }

}   // namespace service

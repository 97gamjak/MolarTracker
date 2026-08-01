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
     * @param watchlist The watchlist object containing the name and other
     * details of the new watchlist
     * @return WatchlistId
     */
    WatchlistId WatchlistService::createWatchlist(
        const finance::Watchlist& watchlist
    )
    {
        return _watchlistRepo->createWatchlist(watchlist);
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
     * @brief Update an existing watchlist
     *
     * @param watchlist The watchlist object containing the updated details of
     * the watchlist
     */
    void WatchlistService::updateWatchlist(const finance::Watchlist& watchlist)
    {
        _watchlistRepo->updateWatchlist(watchlist);
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

}   // namespace service

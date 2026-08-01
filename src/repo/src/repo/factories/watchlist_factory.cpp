#include "watchlist_factory.hpp"

#include "finance/watchlist.hpp"

namespace repo
{

    /**
     * @brief Convert a WatchlistRow database model to a Watchlist domain
     * model
     *
     * @param watchlistRow
     * @param symbols The symbols belonging to this watchlist, loaded
     * separately from the watchlist_instruments table
     * @return finance::Watchlist
     */
    finance::Watchlist WatchlistFactory::toWatchlistDomain(
        const WatchlistRow&     watchlistRow,
        const Set<std::string>& symbols
    )
    {
        finance::Watchlist watchlist{
            watchlistRow.id.value(),
            watchlistRow.name.value(),
            watchlistRow.createdAt.value(),
            symbols
        };

        return watchlist;
    }

    /**
     * @brief Convert a Watchlist domain model to a database row
     *
     * @param watchlist
     * @return WatchlistRow
     */
    WatchlistRow WatchlistFactory::toWatchlistRow(
        const finance::Watchlist& watchlist
    )
    {
        WatchlistRow watchlistRow;

        watchlistRow.id        = watchlist.getId();
        watchlistRow.name      = watchlist.getName();
        watchlistRow.createdAt = watchlist.getCreatedAt();

        return watchlistRow;
    }

}   // namespace repo

#ifndef __REPO__SRC__REPO__FACTORIES__WATCHLIST_FACTORY_HPP__
#define __REPO__SRC__REPO__FACTORIES__WATCHLIST_FACTORY_HPP__

#include <string>
#include <vector>

#include "sql_models/watchlist_row.hpp"

namespace finance
{
    class Watchlist;   // Forward declaration
}   // namespace finance

namespace repo
{

    /**
     * @brief Factory class for converting between Watchlist domain models and
     * WatchlistRow database models
     *
     */
    class WatchlistFactory
    {
       public:
        [[nodiscard]] static finance::Watchlist toWatchlistDomain(
            const WatchlistRow&             watchlistRow,
            const std::vector<std::string>& symbols
        );

        [[nodiscard]] static WatchlistRow toWatchlistRow(
            const finance::Watchlist& watchlist
        );
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__FACTORIES__WATCHLIST_FACTORY_HPP__

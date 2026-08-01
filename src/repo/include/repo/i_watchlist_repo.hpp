#ifndef __REPO__INCLUDE__REPO__I_WATCHLIST_REPO_HPP__
#define __REPO__INCLUDE__REPO__I_WATCHLIST_REPO_HPP__

#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "error/crud_error.hpp"
#include "finance/watchlist.hpp"

namespace repo
{

    /**
     * @brief Interface for Watchlist repository
     *
     */
    class IWatchlistRepo
    {
       public:
        virtual ~IWatchlistRepo() = default;

        /**
         * @brief Create a new, empty watchlist
         *
         * @param watchlist The watchlist object containing the name and other
         * details of the new watchlist
         * @return CrudResult<WatchlistId> The ID of the newly created
         * watchlist, or an error if the operation failed
         */
        [[nodiscard]]
        virtual CrudResult<WatchlistId> createWatchlist(

            const finance::Watchlist& watchlist

        ) = 0;

        /**
         * @brief Get all watchlists, including their symbols
         *
         * @return std::vector<finance::Watchlist>
         */
        [[nodiscard]]
        virtual std::vector<finance::Watchlist> getAllWatchlists() = 0;

        /**
         * @brief Update an existing watchlist
         *
         * @param watchlist The watchlist object containing the updated details
         * of the watchlist
         */
        virtual void updateWatchlist(const finance::Watchlist& watchlist) = 0;

        /**
         * @brief Delete a watchlist and all of its symbol entries
         *
         * @param id
         */
        virtual void deleteWatchlist(WatchlistId id) = 0;
    };

}   // namespace repo

#endif   // __REPO__INCLUDE__REPO__I_WATCHLIST_REPO_HPP__

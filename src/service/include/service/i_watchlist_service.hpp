#ifndef __SERVICE__INCLUDE__SERVICE__I_WATCHLIST_SERVICE_HPP__
#define __SERVICE__INCLUDE__SERVICE__I_WATCHLIST_SERVICE_HPP__

#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "error/crud_error.hpp"

namespace finance
{
    class Watchlist;   // forward declaration
}   // namespace finance

namespace service
{
    /**
     * @brief Interface for Watchlist service
     *
     */
    class IWatchlistService
    {
       public:
        virtual ~IWatchlistService() = default;

        /**
         * @brief Create a new, empty watchlist
         *
         * @param name The display name of the new watchlist
         * @return WatchlistId The ID of the newly created watchlist
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
        virtual std::vector<finance::Watchlist> getAllWatchlists() const = 0;

        /**
         * @brief Update an existing watchlist
         *
         * @param watchlist The watchlist to update
         */
        virtual void updateWatchlist(const finance::Watchlist& watchlist) = 0;

        /**
         * @brief Delete a watchlist and all of its symbol entries
         *
         * @param id
         */
        virtual void deleteWatchlist(WatchlistId id) = 0;
    };

}   // namespace service

#endif   // __SERVICE__INCLUDE__SERVICE__I_WATCHLIST_SERVICE_HPP__

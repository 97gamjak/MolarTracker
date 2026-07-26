#ifndef __SERVICE__INCLUDE__SERVICE__I_WATCHLIST_SERVICE_HPP__
#define __SERVICE__INCLUDE__SERVICE__I_WATCHLIST_SERVICE_HPP__

#include <string>
#include <vector>

#include "config/id_types.hpp"

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
        [[nodiscard]] virtual WatchlistId createWatchlist(
            const std::string& name
        ) = 0;

        /**
         * @brief Get all watchlists, including their symbols
         *
         * @return std::vector<finance::Watchlist>
         */
        [[nodiscard]] virtual std::vector<finance::Watchlist> getAllWatchlists(
        ) const = 0;

        /**
         * @brief Rename an existing watchlist
         *
         * @param id
         * @param newName
         */
        virtual void renameWatchlist(
            WatchlistId        id,
            const std::string& newName
        ) = 0;

        /**
         * @brief Delete a watchlist and all of its symbol entries
         *
         * @param id
         */
        virtual void deleteWatchlist(WatchlistId id) = 0;

        /**
         * @brief Add a symbol to a watchlist
         *
         * @param id
         * @param symbol
         */
        virtual void addSymbol(WatchlistId id, const std::string& symbol) = 0;

        /**
         * @brief Remove a symbol from a watchlist
         *
         * @param id
         * @param symbol
         */
        virtual void removeSymbol(
            WatchlistId        id,
            const std::string& symbol
        ) = 0;
    };

}   // namespace service

#endif   // __SERVICE__INCLUDE__SERVICE__I_WATCHLIST_SERVICE_HPP__

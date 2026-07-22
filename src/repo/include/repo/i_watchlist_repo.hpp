#ifndef __REPO__INCLUDE__REPO__I_WATCHLIST_REPO_HPP__
#define __REPO__INCLUDE__REPO__I_WATCHLIST_REPO_HPP__

#include <string>
#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    class Watchlist;   // Forward declaration
}   // namespace finance

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
        [[nodiscard]] virtual std::vector<finance::Watchlist>
        getAllWatchlists() = 0;

        /**
         * @brief Rename an existing watchlist
         *
         * @param id
         * @param newName
         */
        virtual void renameWatchlist(
            WatchlistId         id,
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
         * @brief Remove a symbol from a watchlist, this is a no-op if the
         * symbol isn't in the watchlist
         *
         * @param id
         * @param symbol
         */
        virtual void removeSymbol(
            WatchlistId         id,
            const std::string& symbol
        ) = 0;
    };

}   // namespace repo

#endif   // __REPO__INCLUDE__REPO__I_WATCHLIST_REPO_HPP__

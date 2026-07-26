#ifndef __STORE__INCLUDE__STORE__I_WATCHLIST_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_WATCHLIST_STORE_HPP__

#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "exceptions/base.hpp"
#include "utils/container/id_id_map.hpp"

namespace finance
{
    class Watchlist;   // Forward declaration
}   // namespace finance

namespace store
{

    class WatchlistStoreException : public MolarTrackerException
    {
       public:
        using MolarTrackerException::MolarTrackerException;
    };

    /**
     * @brief Store for managing watchlists
     *
     */
    class IWatchlistStore
    {
       public:
        virtual ~IWatchlistStore() = default;

        /**
         * @brief Stage the creation of a new, empty watchlist. The watchlist
         * is only persisted once commit() is called.
         *
         * @param name
         * @return WatchlistId The (temporary) ID of the new watchlist
         */
        [[nodiscard]]
        virtual WatchlistId createWatchlist(const std::string& name) = 0;

        /**
         * @brief Get a watchlist by its ID
         *
         * @param id
         * @return std::optional<finance::Watchlist>
         */
        [[nodiscard]]
        virtual std::optional<finance::Watchlist> getWatchlist(
            WatchlistId id
        ) const = 0;

        /**
         * @brief Get all watchlists
         *
         * @return std::vector<finance::Watchlist>
         */
        [[nodiscard]]
        virtual std::vector<finance::Watchlist> getAllWatchlists() const = 0;

        /**
         * @brief Get a mapping of watchlist IDs to their remapped IDs
         *
         * @return const IdIdMap<WatchlistId>&
         */
        [[nodiscard]]
        virtual const IdIdMap<WatchlistId>& getIdRemap() const = 0;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_WATCHLIST_STORE_HPP__

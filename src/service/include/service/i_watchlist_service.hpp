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

        [[nodiscard]] virtual WatchlistId createWatchlist(
            const std::string& name
        ) = 0;

        [[nodiscard]] virtual std::vector<finance::Watchlist>
        getAllWatchlists() const = 0;

        virtual void renameWatchlist(
            WatchlistId         id,
            const std::string& newName
        ) = 0;

        virtual void deleteWatchlist(WatchlistId id) = 0;

        virtual void addSymbol(WatchlistId id, const std::string& symbol) = 0;

        virtual void removeSymbol(
            WatchlistId         id,
            const std::string& symbol
        ) = 0;
    };

}   // namespace service

#endif   // __SERVICE__INCLUDE__SERVICE__I_WATCHLIST_SERVICE_HPP__

#ifndef __SERVICE__SRC__SERVICE__WATCHLIST_SERVICE_HPP__
#define __SERVICE__SRC__SERVICE__WATCHLIST_SERVICE_HPP__

#include <memory>

#include "service/i_watchlist_service.hpp"

namespace repo
{
    class IWatchlistRepo;   // forward declaration
}   // namespace repo

namespace service
{
    /**
     * @brief Implementation of Watchlist service
     *
     */
    class WatchlistService : public IWatchlistService
    {
       private:
        /// reference to the watchlist repository
        std::shared_ptr<repo::IWatchlistRepo> _watchlistRepo;

       public:
        explicit WatchlistService(
            const std::shared_ptr<repo::IWatchlistRepo>& watchlistRepo
        );

        [[nodiscard]]
        CrudResult<WatchlistId> createWatchlist(
            const std::string& name
        ) override;

        [[nodiscard]]
        std::vector<finance::Watchlist> getAllWatchlists() const override;

        void renameWatchlist(
            WatchlistId        id,
            const std::string& newName
        ) override;

        void deleteWatchlist(WatchlistId id) override;

        void addSymbol(WatchlistId id, const std::string& symbol) override;

        void removeSymbol(WatchlistId id, const std::string& symbol) override;
    };

}   // namespace service

#endif   // __SERVICE__SRC__SERVICE__WATCHLIST_SERVICE_HPP__

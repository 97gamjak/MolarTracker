#ifndef __SERVICE__SRC__SERVICE__WATCHLIST_SERVICE_HPP__
#define __SERVICE__SRC__SERVICE__WATCHLIST_SERVICE_HPP__

#include <memory>

#include "finance/watchlist.hpp"
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
            const finance::Watchlist& watchlist
        ) override;

        [[nodiscard]]
        std::vector<finance::Watchlist> getAllWatchlists() const override;

        void updateWatchlist(const finance::Watchlist& watchlist) override;

        void deleteWatchlist(WatchlistId id) override;
    };

}   // namespace service

#endif   // __SERVICE__SRC__SERVICE__WATCHLIST_SERVICE_HPP__

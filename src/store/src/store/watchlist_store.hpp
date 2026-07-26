#ifndef __STORE__SRC__STORE__WATCHLIST_STORE_HPP__
#define __STORE__SRC__STORE__WATCHLIST_STORE_HPP__

#include <memory>

#include "base/base_store.hpp"
#include "config/id_types.hpp"
#include "finance/watchlist.hpp"
#include "store/i_watchlist_store.hpp"

namespace service
{
    class IWatchlistService;   // forward declaration
}   // namespace service

namespace store
{
    /**
     * @brief Store for managing watchlists
     *
     */
    class WatchlistStore : public BaseStore<finance::Watchlist, WatchlistId>,
                           public IWatchlistStore
    {
       private:
        /// reference to the watchlist service
        std::shared_ptr<service::IWatchlistService> _watchlistService;

       public:
        explicit WatchlistStore(
            const std::shared_ptr<service::IWatchlistService>& watchlistService
        );

        [[nodiscard]]
        WatchlistId createWatchlist(const std::string& name) override;

        [[nodiscard]]
        std::optional<finance::Watchlist> getWatchlist(WatchlistId id
        ) const override;

        [[nodiscard]]
        std::vector<finance::Watchlist> getAllWatchlists() const override;

        [[nodiscard]]
        const IdIdMap<WatchlistId>& getIdRemap() const override;

        void renameWatchlist(WatchlistId id, const std::string& newName)
            override;
        void deleteWatchlist(WatchlistId id) override;
        void addSymbol(WatchlistId id, const std::string& symbol) override;
        void removeSymbol(WatchlistId id, const std::string& symbol) override;

        void commit();

        void reload() override;

       private:
        void _refresh();
    };

}   // namespace store

#endif   // __STORE__SRC__STORE__WATCHLIST_STORE_HPP__

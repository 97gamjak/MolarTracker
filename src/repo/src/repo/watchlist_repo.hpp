#ifndef __REPO__SRC__REPO__WATCHLIST_REPO_HPP__
#define __REPO__SRC__REPO__WATCHLIST_REPO_HPP__

#include "base_repo.hpp"
#include "repo/i_watchlist_repo.hpp"

namespace repo
{

    /**
     * @brief Database implementation of Watchlist repository
     *
     */
    class WatchlistRepo : public IWatchlistRepo, public BaseRepo
    {
       public:
        using BaseRepo::BaseRepo;

        [[nodiscard]] WatchlistId createWatchlist(const std::string& name
        ) override;

        [[nodiscard]] std::vector<finance::Watchlist> getAllWatchlists(
        ) override;

        void renameWatchlist(WatchlistId id, const std::string& newName)
            override;

        void deleteWatchlist(WatchlistId id) override;

        void addSymbol(WatchlistId id, const std::string& symbol) override;

        void removeSymbol(WatchlistId id, const std::string& symbol) override;

       private:
        [[nodiscard]] std::vector<std::string> _getSymbols(WatchlistId id);
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__WATCHLIST_REPO_HPP__

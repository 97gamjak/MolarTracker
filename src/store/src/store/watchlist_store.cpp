#include "store/watchlist_store.hpp"

#include <format>

#include "logging/log_macros.hpp"
#include "service/i_watchlist_service.hpp"

REGISTER_LOG_CATEGORY("Store.WatchlistStore");

using finance::HasWatchlistId;

namespace store
{

    /**
     * @brief Construct a new Watchlist Store object
     *
     * @param watchlistService A shared pointer to the watchlist service that
     * the store will use to perform operations related to watchlists
     */
    WatchlistStore::WatchlistStore(
        const std::shared_ptr<service::IWatchlistService>& watchlistService
    )
        : _watchlistService(watchlistService)
    {
        _refresh();
    }

    /**
     * @brief Stage the creation of a new, empty watchlist
     *
     * @param name
     * @return WatchlistId
     */
    WatchlistId WatchlistStore::createWatchlist(const std::string& name)
    {
        const auto newWatchlist =
            finance::Watchlist{WatchlistId::invalid(), name, Timestamp{}};

        return _addEntry(newWatchlist);
    }

    /**
     * @brief Commit any pending changes in the store to the underlying
     * service
     *
     */
    void WatchlistStore::commit()
    {
        LOG_ENTRY;

        for (const auto& entry : _getEntries())
        {
            switch (entry.state)
            {
                case StoreState::New:
                {
                    auto       newEntry = entry;
                    const auto oldId    = newEntry.value.getId();
                    const auto id       = _watchlistService->createWatchlist(
                        newEntry.value.getName()
                    );
                    newEntry.value.setId(id);

                    const auto result = _commitEntry(oldId, newEntry);

                    if (result != StoreResult::Ok)
                    {
                        throw WatchlistStoreException(
                            std::format(
                                "Failed to add watchlist '{}' to database",
                                newEntry.value.getName()
                            )
                        );
                    }

                    break;
                }
                case StoreState::Clean:
                {
                    break;
                }
                case StoreState::Modified:
                case StoreState::Deleted:
                {
                    throw WatchlistStoreException(
                        "Store state " +
                        std::to_string(static_cast<int>(entry.state)) +
                        " not supported yet"
                    );
                }
            }
        }

        _notifyOnCommit();
    }

    /**
     * @brief Refresh the store's data by clearing existing entries and
     * loading watchlists from the underlying service
     *
     */
    void WatchlistStore::_refresh()
    {
        // We really only want to refresh if we are not dirty, otherwise we
        // might lose unsaved changes
        if (isDirty())
            return;

        _clearEntries();

        const auto watchlists = _watchlistService->getAllWatchlists();

        _addCleanEntries(watchlists);
    }

    /**
     * @brief Get a watchlist by its ID
     *
     * @param id
     * @return std::optional<finance::Watchlist>
     */
    std::optional<finance::Watchlist> WatchlistStore::getWatchlist(
        WatchlistId id
    ) const
    {
        const auto options = Options{.filter = HasWatchlistId(id)};

        return _get(options);
    }

    /**
     * @brief Get all watchlists
     *
     * @return std::vector<finance::Watchlist>
     */
    std::vector<finance::Watchlist> WatchlistStore::getAllWatchlists() const
    {
        const auto options = Options{.deletion = DeletionPolicy::ExcludeDelete};

        auto watchlists = _getValues(options);

        return {watchlists.begin(), watchlists.end()};
    }

    /**
     * @brief Get the ID remapping for watchlists
     *
     * @return const IdIdMap<WatchlistId>&
     */
    const IdIdMap<WatchlistId>& WatchlistStore::getIdRemap() const
    {
        return _getIdRemap();
    }

    /**
     * @brief Discard all cached watchlists and reload from the database
     *
     */
    void WatchlistStore::reload()
    {
        LOG_ENTRY;

        _clearEntries();
        _refresh();
    }

}   // namespace store

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
                    const auto id =
                        _watchlistService->createWatchlist(newEntry.value);
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
                {
                    const auto result =
                        _watchlistService->updateWatchlist(entry.value);

                    if (result != WatchlistResult<void>::Ok)
                    {
                        throw WatchlistStoreException(
                            std::format(
                                "Failed to update watchlist '{}' in database",
                                entry.value.getName()
                            )
                        );
                    }

                    if (_commitEntry(entry.value.getId(), entry) !=
                        StoreResult::Ok)
                    {
                        throw WatchlistStoreException(
                            std::format(
                                "Failed to update cached watchlist '{}' after "
                                "update",
                                entry.value.getName()
                            )
                        );
                    }

                    break;
                }
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
     * @brief Rename an existing watchlist. Persisted immediately (not
     * staged) since the repo/service API is a single atomic operation, not a
     * value to diff against a previous state at commit time.
     *
     * @param id
     * @param newName
     */
    void WatchlistStore::renameWatchlist(
        WatchlistId        id,
        const std::string& newName
    )
    {
        auto watchlist = getWatchlist(id);
        if (!watchlist)
            throw WatchlistStoreException(
                "Watchlist not found: " + id.toString()
            );

        _watchlistService->renameWatchlist(id, newName);
        watchlist->setName(newName);

        if (_updateEntry(*watchlist, StoreState::Clean) != StoreResult::Ok)
            throw WatchlistStoreException(
                "Failed to update cached watchlist after rename"
            );
    }

    /**
     * @brief Delete a watchlist and all of its symbol entries immediately.
     *
     * @param id
     */
    void WatchlistStore::deleteWatchlist(WatchlistId id)
    {
        _watchlistService->deleteWatchlist(id);
        _removeEntry(id);
    }

    /**
     * @brief Add a symbol to a watchlist immediately.
     *
     * @param id
     * @param symbol
     *
     * @return WatchlistResult<void> Returns an error if the watchlist does not
     * exist or if the symbol already exists in the watchlist.
     */
    WatchlistResult<void> WatchlistStore::addSymbol(
        WatchlistId        id,
        const std::string& symbol
    )
    {
        Options options{
            .filter   = HasWatchlistId(id),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        auto entry = _getEntry(options);

        if (!entry.has_value())
        {
            const auto error = WatchlistError(
                WatchlistErrorType::WatchlistNotFound,
                "Watchlist not found: " + id.toString()
            );

            LOG_ERROR(error.toString());
            return error;
        }

        if (!entry->value.addSymbol(symbol))
        {
            const auto error = WatchlistError(
                WatchlistErrorType::WatchlistSymbolAlreadyExists,
                "Symbol '" + symbol + "' already exists in watchlist '" +
                    entry->value.getName() + "'"
            );

            LOG_ERROR(error.toString());
            return error;
        }

        // only update the entries state if it was previously clean, otherwise
        // it is already marked as modified or new and will be committed. If it
        // is deleted, we should never reach this point since we exclude deleted
        // entries in the options above.
        const auto newState = entry->state == StoreState::Clean
                                  ? StoreState::Modified
                                  : entry->state;

        if (_updateEntry(entry->value, newState) != StoreResult::Ok)
        {
            const auto error = WatchlistError(
                WatchlistErrorType::WatchlistSymbolAlreadyExists,
                "Failed to update cached watchlist after addSymbol"
            );

            LOG_ERROR(error.toString());
            return error;
        }

        return {};
    }

    /**
     * @brief Remove a symbol from a watchlist immediately.
     *
     * @param id
     * @param symbol
     *
     * @return WatchlistResult<void> Returns an error if the watchlist does not
     * exist or if the symbol does not exist in the watchlist.
     */
    WatchlistResult<void> WatchlistStore::removeSymbol(
        WatchlistId        id,
        const std::string& symbol
    )
    {
        const auto options = Options{
            .filter   = HasWatchlistId(id),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        auto watchlist = _getEntry(options);

        if (!watchlist.has_value())
        {
            const auto error = WatchlistError(
                WatchlistErrorType::WatchlistNotFound,
                "Watchlist not found: " + id.toString()
            );
            LOG_ERROR(error.toString());
            return error;
        }

        if (!watchlist->value.removeSymbol(symbol))
        {
            const auto error = WatchlistError(
                WatchlistErrorType::WatchlistSymbolNotFound,
                "Symbol '" + symbol + "' not found in watchlist '" +
                    watchlist->value.getName() + "'"
            );
            LOG_ERROR(error.toString());
            return error;
        }

        // only update the entries state if it was previously clean, otherwise
        // it is already marked as modified or new and will be committed. If it
        // is deleted, we should never reach this point since we exclude deleted
        // entries in the options above.
        const auto newState = watchlist->state == StoreState::Clean
                                  ? StoreState::Modified
                                  : watchlist->state;

        if (_updateEntry(watchlist->value, newState) != StoreResult::Ok)
        {
            const auto error = WatchlistError(
                WatchlistErrorType::WatchlistSymbolNotFound,
                "Failed to update cached watchlist after removeSymbol"
            );
            LOG_ERROR(error.toString());
            return error;
        }

        return {};
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

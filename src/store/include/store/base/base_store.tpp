#ifndef __STORE__INCLUDE__STORE__BASE__BASE_STORE_TPP__
#define __STORE__INCLUDE__STORE__BASE__BASE_STORE_TPP__

#include <algorithm>
#include <ranges>

#include "base_store.hpp"
#include "config/id_types.hpp"
#include "config/signal_tags.hpp"
#include "logging/log_macros.hpp"
#include "store/base/store_state.hpp"

REGISTER_LOG_CATEGORY("Store.BaseStore");

namespace store
{

    /**
     * @brief Construct a new Base Store< T,  Id Type>:: Base Store object
     *
     * @tparam T
     * @tparam IdType
     * @param fullCache
     */
    template <typename T, typename IdType>
    BaseStore<T, IdType>::BaseStore(bool fullCache) : _fullCache(fullCache)
    {
    }

    /**
     * @brief Checks if an entry with the given ID is marked as deleted in the
     * store.
     *
     * @tparam T
     * @tparam IdType
     * @param id
     * @return true
     * @return false
     */
    template <typename T, typename IdType>
    bool BaseStore<T, IdType>::_isDeleted(IdType id) const
    {
        const auto* entry = _findEntry(id);
        return entry != nullptr && entry->state == StoreState::Deleted;
    }

    /**
     * @brief Checks if the store has any entries that are not marked as
     * deleted.
     *
     * @tparam T
     * @tparam IdType
     * @return true
     * @return false
     */
    template <typename T, typename IdType>
    bool BaseStore<T, IdType>::_hasNonDeletedEntries() const
    {
        return std::ranges::any_of(
            _entries,
            [](const auto& entry) { return entry.state != StoreState::Deleted; }
        );
    }

    /**
     * @brief Checks if the store is dirty, meaning it has any entries that are
     * not in the Clean state (i.e., they are either New or Deleted).
     *
     * @tparam T
     * @tparam IdType
     * @return true
     * @return false
     */
    template <typename T, typename IdType>
    bool BaseStore<T, IdType>::isDirty() const
    {
        return std::ranges::any_of(
            _entries,
            [](const auto& entry) { return entry.state != StoreState::Clean; }
        );
    }

    /**
     * @brief Checks if all entries in the store are dirty, meaning none of
     * them are in the Clean state.
     *
     * @tparam T
     * @tparam IdType
     * @return true
     * @return false
     */
    template <typename T, typename IdType>
    bool BaseStore<T, IdType>::allDirty() const
    {
        return std::ranges::all_of(
            _entries,
            [](const auto& entry) { return entry.state != StoreState::Clean; }
        );
    }

    /**
     * @brief Finds an entry in the store that matches the given predicate and
     * returns a pointer to it, or nullptr if not found.
     *
     * @tparam T
     * @tparam IdType
     * @param id
     * @return BaseStore<T, IdType>::Entry*
     */
    template <typename T, typename IdType>
    auto BaseStore<T, IdType>::_findEntry(IdType id) -> Entry*
    {
        auto it = std::ranges::find_if(
            _entries,
            [id](const auto& entry) { return getId(entry.value) == id; }
        );

        return it != _entries.end() ? &(*it) : nullptr;
    }

    /**
     * @brief Retrieves the value of an entry that matches the given options,
     * returning it as an optional. If no matching entry is found, returns
     * std::nullopt.
     *
     * @tparam T
     * @tparam IdType
     * @param options
     * @return std::optional<T>
     */
    template <typename T, typename IdType>
    std::optional<T> BaseStore<T, IdType>::_get(Options options) const
    {
        auto it = std::ranges::find_if(
            _entries,
            [&options](const auto& entry) { return options.eval(entry); }
        );

        return it != _entries.end() ? std::optional<T>{it->value}
                                    : std::nullopt;
    }

    /**
     * @brief Retrieves a mutable reference to the entry that matches the given
     * options, if it exists.
     *
     * @tparam T
     * @tparam IdType
     * @param options
     * @return std::optional<Entry&>
     */
    template <typename T, typename IdType>
    auto BaseStore<T, IdType>::_getEntry(Options options) const
    {
        auto it = std::ranges::find_if(
            _entries,
            [&options](const auto& entry) { return options.eval(entry); }
        );

        return it != _entries.end() ? std::optional<Entry>{*it} : std::nullopt;
    }

    /**
     * @brief Retrieves a set of IDs from the entries in the store that match
     * the given options.
     *
     * @tparam T
     * @tparam IdType
     * @param options
     * @return idSet<IdType>
     */
    template <typename T, typename IdType>
    idSet<IdType> BaseStore<T, IdType>::_getIds(Options options) const
    {
        idSet<IdType> ids;
        for (const auto& entry : _entries)
            if (options.eval(entry))
                ids.insert(getId(entry.value));

        return ids;
    }

    /**
     * @brief Retrieves a const reference to the collection of entries in the
     * store.
     *
     * @tparam T
     * @tparam IdType
     * @param options
     * @return const std::vector<typename BaseStore<T, IdType>::Entry>&
     */
    template <typename T, typename IdType>
    auto BaseStore<T, IdType>::_getEntries(Options options) const
    {
        // pipe operator not working here due _Partial adaptor invocable
        // constraints -- NO IDEA WHY
        return std::ranges::filter_view(
            _entries,
            [options](const auto& entry) { return options.eval(entry); }
        );
    }

    /**
     * @brief Retrieves a collection of values from the entries in the store
     * that match the given options, returning them as a vector.
     *
     * @tparam T
     * @tparam IdType
     * @param options
     * @return std::vector<T>
     */
    template <typename T, typename IdType>
    auto BaseStore<T, IdType>::_getValues(Options options) const
    {
        return _getEntries(options) |
               std::views::transform([](const auto& entry)
                                     { return entry.value; });
    }

    /**
     * @brief Generates a new unique ID for an entry in the store by
     * incrementing from 0 until it finds an ID that is not already used by any
     * existing entry.
     *
     * @tparam T
     * @tparam IdType
     * @return IdType
     */
    template <typename T, typename IdType>
    IdType BaseStore<T, IdType>::_generateNewId()
    {
        return _idSequence.next();
    }

    /**
     * @brief Adds a new entry to the store with the given value and state. If
     * the state is not Clean, marks the store as potentially dirty.
     *
     * @tparam T
     * @tparam IdType
     * @param value
     *
     * @return IdType The ID of the newly added entry.
     */
    template <typename T, typename IdType>
    IdType BaseStore<T, IdType>::_addEntry(T value)
    {
        _markPotentiallyDirty();

        value.setId(_generateNewId());

        _entries.push_back(Entry{value, StoreState::New});

        _added.push_back(value);
        _notifyAdded(false);

        return value.getId();
    }

    /**
     * @brief Adds a collection of new entries to the store with the given
     * values. Marks the store as potentially dirty.
     *
     * @tparam T
     * @tparam IdType
     * @param value
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_addCleanEntries(const std::vector<T>& value)
    {
        for (const auto& item : value)
            _entries.push_back(Entry{item, StoreState::Clean});

        _notifyStoreChanged(false);
    }

    /**
     * @brief Updates an existing entry in the store with the given value and
     * state. If the entry is not found, returns NotFound.
     *
     * @tparam T
     * @tparam IdType
     * @param value
     * @param state
     * @return StoreResult
     */
    template <typename T, typename IdType>
    StoreResult BaseStore<T, IdType>::_updateEntry(
        const T&   value,
        StoreState state
    )
    {
        if (state != StoreState::Clean)
            _markPotentiallyDirty();

        auto entry = _findEntry(value.getId());
        if (!entry)
            return StoreResult::NotFound;

        entry->value = value;
        entry->state = state;

        _updated.push_back(entry->value);
        _notifyUpdated(false);
        return StoreResult::Ok;
    }

    /**
     * @brief Removes an entry with the given ID from the store. If an entry
     * with the specified ID is found and removed, marks the store as
     * potentially dirty.
     *
     * @tparam T
     * @tparam IdType
     * @param id
     * @return StoreResult
     */
    template <typename T, typename IdType>
    StoreResult BaseStore<T, IdType>::_removeEntry(IdType id)
    {
        LOG_ENTRY;

        const auto* entry = _findEntry(id);

        if (entry == nullptr)
            return StoreResult::NotFound;

        auto [beg, end] = std::ranges::remove_if(
            _entries,
            [id](const auto& entry_) { return entry_.value.getId() == id; }
        );
        _entries.erase(beg, end);

        return StoreResult::Ok;
    }

    /**
     * @brief Deletes an entry with the given ID from the store. If an entry
     * with the specified ID is found and deleted, marks the store as
     * potentially dirty.
     *
     * @tparam T
     * @tparam IdType
     * @param id
     * @return StoreResult
     */
    template <typename T, typename IdType>
    StoreResult BaseStore<T, IdType>::_deleteEntry(IdType id)
    {
        LOG_ENTRY;

        const auto* entry = _findEntry(id);

        if (entry == nullptr)
            return StoreResult::NotFound;

        // if entry is new, remove it directly
        if (entry->state == StoreState::New)
            return _removeEntry(id);

        const auto result = _updateEntry(entry->value, StoreState::Deleted);

        if (result != StoreResult::Ok)
            return result;

        _removed.push_back(id);
        _notifyRemoved(false);
        _markPotentiallyDirty();

        return StoreResult::Ok;
    }

    /**
     * @brief Clears all entries from the store. If any entries are cleared,
     * marks the store as potentially dirty.
     *
     * @tparam T
     * @tparam IdType
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_clearEntries()
    {
        LOG_ENTRY;

        if (!_entries.empty())
        {
            _markPotentiallyDirty();
            _entries.clear();
        }
    }

    /**
     * @brief Marks the store as potentially dirty, indicating that it has
     * unsaved changes. Emits a signal to notify subscribers of the dirty state
     * change.
     *
     * @tparam T
     * @tparam IdType
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_markPotentiallyDirty()
    {
        _isPotentiallyDirty = true;
        this->template notify<OnDirtyChanged>(true);
    }

    /**
     * @brief Clears the potentially dirty state of the store, indicating that
     * there are no unsaved changes. Emits a signal to notify subscribers of the
     * dirty state change.
     *
     * @tparam T
     * @tparam IdType
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::clearPotentiallyDirty()
    {
        _isPotentiallyDirty = false;
        this->template notify<OnDirtyChanged>(false);
    }

    /**
     * @brief Evaluates whether an entry should be included based on the given
     * deletion policy. This is used to determine whether entries that are
     * marked as deleted should be included in certain operations, such as
     * retrieval or display.
     *
     * @tparam T
     * @tparam IdType
     * @param entry
     * @param policy
     * @return true if the entry should be included based on the deletion
     * policy, false otherwise.
     */
    template <typename T, typename IdType>
    bool BaseStore<T, IdType>::_evalDeletionPolicy(
        const Entry&   entry,
        DeletionPolicy policy
    )
    {
        return policy == DeletionPolicy::IncludeDelete ||
               entry.state != StoreState::Deleted;
    }

    /**
     * @brief Commits the changes made to a temporary entry by replacing it with
     * a persisted entry. If the entry is not found, returns NotFound.
     *
     * @tparam T
     * @tparam IdType
     * @param tempId
     * @param persistedValue
     * @return StoreResult
     */
    template <typename T, typename IdType>
    StoreResult BaseStore<T, IdType>::_commitEntry(
        IdType       tempId,
        const Entry& persistedValue
    )
    {
        auto entry = _findEntry(tempId);
        if (!entry)
            return StoreResult::NotFound;

        if (tempId != getId(persistedValue.value) &&
            persistedValue.state == StoreState::New)
            _idRemap[tempId] = getId(persistedValue.value);

        if (persistedValue.state == StoreState::New ||
            persistedValue.state == StoreState::Modified)
            entry->value = persistedValue.value;

        if (persistedValue.state == StoreState::Deleted)
            _removeEntry(getId(persistedValue.value));

        entry->state = StoreState::Clean;

        // when committing we don't want single notifications
        return StoreResult::Ok;
    }

    /**
     * @brief Checks if the store is fully cached.
     *
     * @tparam T
     * @tparam IdType
     * @return true if the store is fully cached, false otherwise.
     */
    template <typename T, typename IdType>
    bool BaseStore<T, IdType>::isFullCache() const
    {
        return _fullCache;
    }

    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_logCache(
        const std::string& category,
        LogLevel           level
    )
    {
        if (logging::LogManager::getInstance().isEnabled(category, level))
        {
            EXPLICIT_LOG(
                level,
                category,
                std::format("Cache contents ({}):", _entries.size())
            );

            for (const auto& entry : _entries)
            {
                EXPLICIT_LOG(
                    level,
                    category,
                    std::format(
                        "Cache: {{value: {}, state: {}}}",
                        entry.value.toString(),
                        StoreStateMeta::toString(entry.state)
                    )
                );
            }
        }
    }

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__BASE__BASE_STORE_TPP__
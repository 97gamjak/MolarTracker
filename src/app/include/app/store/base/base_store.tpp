#ifndef __APP__STORE__BASE__BASE_STORE_TPP__
#define __APP__STORE__BASE__BASE_STORE_TPP__

#include <algorithm>

#include "base_store.hpp"
#include "config/id_types.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("App.Store.BaseStore");

namespace app
{

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
     * @brief Finds an entry in the store by its ID and returns a pointer to it,
     * or nullptr if not found.
     *
     * @tparam T
     * @tparam IdType
     * @param id
     * @return BaseStore<T, IdType>::Entry*
     */
    template <typename T, typename IdType>
    typename BaseStore<T, IdType>::Entry* BaseStore<T, IdType>::_findEntry(
        IdType id
    )
    {
        return _findEntry([id](const auto& entry)
                          { return getId(entry.value) == id; });
    }

    /**
     * @brief Finds an entry in the store that matches the given predicate and
     * returns a pointer to it, or nullptr if not found.
     *
     * @tparam T
     * @tparam IdType
     * @param pred
     * @return BaseStore<T, IdType>::Entry*
     */
    template <typename T, typename IdType>
    typename BaseStore<T, IdType>::Entry* BaseStore<T, IdType>::_findEntry(
        Predicate<Entry> pred
    )
    {
        auto it = std::ranges::find_if(
            _entries,
            [&pred](const auto& entry) { return pred(entry); }
        );

        return it != _entries.end() ? &(*it) : nullptr;
    }

    /**
     * @brief Retrieves the value of an entry that matches the given predicate,
     * returning it as an optional. If no matching entry is found, returns
     * std::nullopt.
     *
     * @tparam T
     * @tparam IdType
     * @param pred
     * @return std::optional<T>
     */
    template <typename T, typename IdType>
    std::optional<T> BaseStore<T, IdType>::_get(Predicate<Entry> pred) const
    {
        auto it = std::ranges::find_if(
            _entries,
            [&pred](const auto& entry) { return pred(entry); }
        );

        return it != _entries.end() ? std::optional<T>{it->value}
                                    : std::nullopt;
    }

    /**
     * @brief Retrieves a const reference to the collection of entries in the
     * store.
     *
     * @tparam T
     * @tparam IdType
     * @return const std::vector<typename BaseStore<T, IdType>::Entry>&
     */
    template <typename T, typename IdType>
    [[nodiscard]] const std::vector<typename BaseStore<T, IdType>::Entry>& BaseStore<
        T,
        IdType>::_getEntries() const
    {
        return _entries;
    }

    /**
     * @brief Retrieves a reference to the collection of entries in the store.
     *
     * @tparam T
     * @tparam IdType
     * @return std::vector<typename BaseStore<T, IdType>::Entry>&
     */
    template <typename T, typename IdType>
    [[nodiscard]] std::vector<typename BaseStore<T, IdType>::Entry>& BaseStore<
        T,
        IdType>::_getEntries()
    {
        return _entries;
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
    IdType BaseStore<T, IdType>::_generateNewId() const
    {
        IdType newId{0};
        while (std::ranges::any_of(
            _entries,
            [&newId](const auto& entry) { return getId(entry.value) == newId; }
        ))
        {
            newId = IdType{newId.value() + 1};
        }
        return newId;
    }

    /**
     * @brief Adds a new entry to the store with the given value and state. If
     * the state is not Clean, marks the store as potentially dirty.
     *
     * @tparam T
     * @tparam IdType
     * @param value
     * @param state
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_addEntry(const T& value, StoreState state)
    {
        if (state != StoreState::Clean)
            _markPotentiallyDirty();

        _entries.push_back(Entry{value, state});
    }

    /**
     * @brief Removes an entry with the given ID from the store. If an entry
     * with the specified ID is found and removed, marks the store as
     * potentially dirty.
     *
     * @tparam T
     * @tparam IdType
     * @param id
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_removeEntry(IdType id)
    {
        LOG_ENTRY;

        auto [beg, end] = std::ranges::remove_if(
            _entries,
            [id](const auto& entry) { return entry.value.getId() == id; }
        );
        _entries.erase(beg, end);
    }

    /**
     * @brief Removes all entries from the store that are not in the Clean state
     * (i.e., they are either New or Deleted). If any entries are removed, marks
     * the store as potentially dirty.
     *
     * @tparam T
     * @tparam IdType
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_cleanEntries()
    {
        LOG_ENTRY;

        auto [beg, end] = std::ranges::remove_if(
            _entries,
            [](const auto& entry) { return entry.state != StoreState::Clean; }
        );
        _entries.erase(beg, end);
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
        DirtyObservable::_emit<OnDirtyChanged>(true);
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
        DirtyObservable::_emit<OnDirtyChanged>(false);
    }

    /**
     * @brief Subscribes a callback function to be called when the dirty state
     * of the store changes. The callback will receive a boolean indicating
     * whether the store is now dirty (true) or not (false).
     *
     * @tparam T
     * @tparam IdType
     * @param func
     * @param user
     * @return Connection
     */
    template <typename T, typename IdType>
    [[nodiscard]] Connection BaseStore<T, IdType>::subscribeToDirty(
        OnDirtyChanged::func func,
        void*                user
    )
    {
        return DirtyObservable::template on<OnDirtyChanged>(func, user);
    }

    /**
     * @brief Appends a mapping of old ID to new ID to the collection of changed
     * IDs in the store. This is used to track changes to entry IDs, such as
     * when an entry is updated and its ID changes. Marks the store as
     * potentially dirty.
     *
     * @tparam T
     * @tparam IdType
     * @param oldId
     * @param newId
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_appendChangedIds(IdType oldId, IdType newId)
    {
        _changedIds[oldId] = newId;
    }

    /**
     * @brief Clears the collection of changed IDs in the store, indicating that
     * there are no tracked changes to entry IDs. Marks the store as
     * potentially dirty.
     *
     * @tparam T
     * @tparam IdType
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_clearChangedIds()
    {
        _changedIds.clear();
    }

    /**
     * @brief Retrieves the collection of changed IDs in the store, which is a
     * mapping of old IDs to new IDs for entries that have been updated. This
     * allows callers to understand how entry IDs have changed as a result of
     * updates to the store.
     *
     * @tparam T
     * @tparam IdType
     * @return const std::unordered_map<IdType, IdType>&
     */
    template <typename T, typename IdType>
    [[nodiscard]] const std::unordered_map<IdType, IdType>& BaseStore<
        T,
        IdType>::getChangedIds() const
    {
        return _changedIds;
    }

}   // namespace app

#endif   // __APP__STORE__BASE__BASE_STORE_TPP__
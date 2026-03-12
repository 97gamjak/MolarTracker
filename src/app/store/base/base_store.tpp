#ifndef __APP__STORE__BASE__BASE_STORE_TPP__
#define __APP__STORE__BASE__BASE_STORE_TPP__

#include <algorithm>

#include "base_store.hpp"

namespace app
{

    template <typename T, typename IdType>
    bool BaseStore<T, IdType>::_isDeleted(IdType id) const
    {
        const auto* entry = _findEntry(id);
        return entry != nullptr && entry->state == StoreState::Deleted;
    }

    template <typename T, typename IdType>
    bool BaseStore<T, IdType>::_hasNonDeletedEntries() const
    {
        return std::ranges::any_of(
            _entries,
            [](const auto& entry) { return entry.state != StoreState::Deleted; }
        );
    }

    template <typename T, typename IdType>
    bool BaseStore<T, IdType>::isDirty() const
    {
        return std::ranges::any_of(
            _entries,
            [](const auto& entry) { return entry.state != StoreState::Clean; }
        );
    }

    template <typename T, typename IdType>
    typename BaseStore<T, IdType>::Entry* BaseStore<T, IdType>::_findEntry(
        IdType id
    )
    {
        return _findEntry([id](const auto& entry)
                          { return entry.value.getId() == id; });
    }

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

    template <typename T, typename IdType>
    [[nodiscard]] const std::vector<typename BaseStore<T, IdType>::Entry>& BaseStore<
        T,
        IdType>::_getEntries() const
    {
        return _entries;
    }

    template <typename T, typename IdType>
    [[nodiscard]] std::vector<typename BaseStore<T, IdType>::Entry>& BaseStore<
        T,
        IdType>::_getEntries()
    {
        return _entries;
    }

    template <typename T, typename IdType>
    IdType BaseStore<T, IdType>::_generateNewId() const
    {
        IdType newId{0};
        while (std::ranges::any_of(
            _entries,
            [&newId](const auto& entry) { return entry.value.getId() == newId; }
        ))
        {
            newId = IdType{newId.value() + 1};
        }
        return newId;
    }

    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_addEntry(const T& value, StoreState state)
    {
        if (state != StoreState::Clean)
            _markPotentiallyDirty();

        _entries.push_back(Entry{value, state});
    }

    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_removeEntry(IdType id)
    {
        auto [beg, end] = std::ranges::remove_if(
            _entries,
            [id](const auto& entry) { return entry.value.getId() == id; }
        );
        _entries.erase(beg, end);
    }

    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_cleanEntries()
    {
        auto [beg, end] = std::ranges::remove_if(
            _entries,
            [](const auto& entry) { return entry.state != StoreState::Clean; }
        );
        _entries.erase(beg, end);
    }

    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_clearEntries()
    {
        if (!_entries.empty())
        {
            _markPotentiallyDirty();
            _entries.clear();
        }
    }

    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_markPotentiallyDirty()
    {
        _isPotentiallyDirty = true;
        DirtyObservable::_emit<OnDirtyChanged>(true);
    }

    template <typename T, typename IdType>
    void BaseStore<T, IdType>::clearPotentiallyDirty()
    {
        _isPotentiallyDirty = false;
        DirtyObservable::_emit<OnDirtyChanged>(false);
    }

    template <typename T, typename IdType>
    [[nodiscard]] Connection BaseStore<T, IdType>::subscribeToDirty(
        OnDirtyChanged::func func,
        void*                user
    )
    {
        return DirtyObservable::template on<OnDirtyChanged>(func, user);
    }

}   // namespace app

#endif   // __APP__STORE__BASE__BASE_STORE_TPP__
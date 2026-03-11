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
    bool BaseStore<T, IdType>::_isDirty() const
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
        auto it = std::ranges::find_if(
            _entries,
            [&id](const auto& entry) { return entry.value.getId() == id; }
        );

        return it != _entries.end() ? &(*it) : nullptr;
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
    void BaseStore<T, IdType>::_removeEntry(IdType id)
    {
        auto [beg, end] = std::ranges::remove_if(
            _entries,
            [id](const auto& entry) { return entry.value.getId() == id; }
        );
        _entries.erase(beg, end);
    }

    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_markPotentiallyDirty()
    {
        _isPotentiallyDirty = true;
        DirtyObservable::_emit(true);
    }

    template <typename T, typename IdType>
    void BaseStore<T, IdType>::clearPotentiallyDirty()
    {
        _isPotentiallyDirty = false;
        DirtyObservable::_emit(false);
    }

}   // namespace app

#endif   // __APP__STORE__BASE__BASE_STORE_TPP__
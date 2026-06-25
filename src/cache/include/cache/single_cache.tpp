#ifndef __CACHE__INCLUDE__CACHE__SINGLE_CACHE_TPP__
#define __CACHE__INCLUDE__CACHE__SINGLE_CACHE_TPP__

#include "single_cache.hpp"

namespace cache
{
    template <typename Key, typename Value>
    std::shared_ptr<const Value> SingleCache<Key, Value>::get(const Key& key)
    {
        if (_entries.contains(key))
        {
            this->recordHit();
            return _entries[key];
        }

        this->recordMiss();
        auto value = _load(key);
        if (value)
        {
            _entries[key] = value;
        }
        return value;
    }

    template <typename Key, typename Value>
    IdObjectMap<std::shared_ptr<const Value>> SingleCache<Key, Value>::getAll()
    {
        if (_fullyLoaded)
            return _entries;

        _fullyLoaded       = true;
        const auto entries = _loadAll();
        for (const auto& [key, value] : entries)
        {
            if (!maxCapacityReached())
                _entries[key] = value;
            else
                _fullyLoaded = false;
        }
        return entries;
    }

    template <typename Key, typename Value>
    void SingleCache<Key, Value>::invalidate(const Key& key)
    {
        this->recordEviction();
        _entries.removeUnchecked(key);
    }

    template <typename Key, typename Value>
    void SingleCache<Key, Value>::clear()
    {
        this->recordEviction();
        _entries.clear();
    }

    template <typename Key, typename Value>
    std::size_t SingleCache<Key, Value>::size() const
    {
        return _entries.size();
    }

    template <typename Key, typename Value>
    void SingleCache<Key, Value>::_onStoreAdded(
        const Key&                   key,
        std::shared_ptr<const Value> value
    )
    {
        _entries[key] = value;
        Base::_observable.template notify<OnAdded<Key, Value>>(
            key,
            value.get()
        );
    }

    template <typename Key, typename Value>
    void SingleCache<Key, Value>::_onStoreUpdated(
        const Key&                   key,
        std::shared_ptr<const Value> value
    )
    {
        _entries[key] = value;
        Base::_observable.template notify<OnUpdated<Key, Value>>(
            key,
            value.get()
        );
    }

    template <typename Key, typename Value>
    void SingleCache<Key, Value>::_onStoreRemoved(const Key& key)
    {
        _entries.removeUnchecked(key);
        this->recordEviction();
        Base::_observable.template notify<OnRemoved<Key>>(key);
    }

    template <typename Key, typename Value>
    Connection SingleCache<Key, Value>::subscribeToAdded(
        typename OnAdded<Key, Value>::func callback,
        void*                              subscriber
    )
    {
        return Base::_observable.template subscribe<OnAdded<Key, Value>>(
            callback,
            subscriber
        );
    }

    template <typename Key, typename Value>
    Connection SingleCache<Key, Value>::subscribeToUpdated(
        typename OnUpdated<Key, Value>::func callback,
        void*                                subscriber
    )
    {
        return Base::_observable.template subscribe<OnUpdated<Key, Value>>(
            callback,
            subscriber
        );
    }

    template <typename Key, typename Value>
    Connection SingleCache<Key, Value>::subscribeToRemoved(
        typename OnRemoved<Key>::func callback,
        void*                         subscriber
    )
    {
        return Base::_observable.template subscribe<OnRemoved<Key>>(
            callback,
            subscriber
        );
    }
}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__SINGLE_CACHE_TPP__
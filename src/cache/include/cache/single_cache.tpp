#ifndef __CACHE__INCLUDE__CACHE__SINGLE_CACHE_TPP__
#define __CACHE__INCLUDE__CACHE__SINGLE_CACHE_TPP__

#include "single_cache.hpp"

namespace cache
{

    template <typename Key, typename Value>
    std::shared_ptr<const Value> SingleCache<Key, Value>::get(const Key& key)
    {
        if (const auto& entry = find(key))
            return entry;

        auto value = _load(key);
        _add(key, value);
        return value;
    }

    template <typename Key, typename Value>
    std::shared_ptr<const Value> SingleCache<Key, Value>::find(const Key& key)
    {
        if (_entries.contains(key))
        {
            this->_recordHit();
            return _entries[key];
        }

        this->_recordMiss();
        return nullptr;
    }

    template <typename Key, typename Value>
    std::shared_ptr<const Value> SingleCache<Key, Value>::get(
        std::function<std::shared_ptr<const Value>()> findFunc,
        std::function<std::shared_ptr<const Value>()> loadFunc
    )
    {
        if (const auto& entry = find(findFunc))
            return entry;

        auto       value = loadFunc();
        const auto key   = value->getId();
        _add(key, value);
        return value;
    }

    template <typename Key, typename Value>
    std::shared_ptr<const Value> SingleCache<Key, Value>::find(
        std::function<std::shared_ptr<const Value>()> findFunc
    )
    {
        if (const auto& entry = findFunc())
        {
            const auto key = entry->getId();
            this->_recordHit();
            return _entries[key];
        }

        this->_recordMiss();
        return nullptr;
    }

    template <typename Key, typename Value>
    IdObjectMap<std::shared_ptr<const Value>> SingleCache<Key, Value>::getBulk(
        std::function<IdObjectMap<std::shared_ptr<const Value>>()> func
    )
    {
        if (_fullyLoaded)
            return _entries;

        _fullyLoaded       = true;
        const auto entries = func();
        for (const auto& [key, value] : entries)
            _add(key, value);

        return entries;
    }

    template <typename Key, typename Value>
    void SingleCache<Key, Value>::invalidate(const Key& key)
    {
        this->_recordEviction();
        _entries.removeUnchecked(key);
    }

    template <typename Key, typename Value>
    void SingleCache<Key, Value>::clear()
    {
        this->_recordEviction();
        _fullyLoaded = false;
        _entries.clear();
    }

    template <typename Key, typename Value>
    std::size_t SingleCache<Key, Value>::size() const
    {
        return _entries.size();
    }

    template <typename Key, typename Value>
    void SingleCache<Key, Value>::_addAndNotify(
        const Key&                   key,
        std::shared_ptr<const Value> value
    )
    {
        _add(key, value);

        Base::_observable.template notify<OnAdded<Key, Value>>(key, value);
        Base::_observable.template notify<OnChanged>();
    }

    template <typename Key, typename Value>
    void SingleCache<Key, Value>::_add(
        const Key&                   key,
        std::shared_ptr<const Value> value
    )
    {
        bool added = false;
        if (!_maxCapacityReached())
        {
            _entries[key] = value;
            added         = true;
        }

        _fullyLoaded &= added;

        if (added)
            _onAdded(key, value);
    }

    template <typename Key, typename Value>
    void SingleCache<Key, Value>::_update(
        const Key&                   key,
        std::shared_ptr<const Value> value
    )
    {
        if (_entries.contains(key))
        {
            const auto& oldValue = _entries[key];
            _entries[key]        = value;
            _onUpdated(key, oldValue, value);
            Base::_observable.template notify<OnUpdated<Key, Value>>(
                key,
                value
            );
            Base::_observable.template notify<OnChanged>();
        }
        else
        {
            _add(key, value);
        }
    }

    template <typename Key, typename Value>
    void SingleCache<Key, Value>::_remove(const Key& key)
    {
        if (_entries.remove(key))
        {
            _onRemoved(key);
            this->_recordEviction();
        }

        Base::_observable.template notify<OnRemoved<Key>>(key);
        Base::_observable.template notify<OnChanged>();
    }

    template <typename Key, typename Value>
    void SingleCache<Key, Value>::_changeId(
        const Key& oldKey,
        const Key& newKey
    )
    {
        if (!_entries.contains(oldKey) || oldKey == newKey)
            return;

        if (oldKey > 0 || newKey < 0)
        {
            throw std::runtime_error(
                std::format(
                    "Invalid ID change from {} to {}. Old ID must be positive "
                    "and new ID must be negative.",
                    oldKey.toString(),
                    newKey.toString()
                )
            );
        }

        // this works because updated ids are always positive while deleted ids
        // are negative, so we have no collision when remapping ids
        const auto& value = _entries[oldKey];
        _entries[newKey]  = value;
        _entries.removeUnchecked(oldKey);

        _onIdChanged(oldKey, newKey);

        Base::_observable.template notify<OnIdChange<Key, Value>>(
            oldKey,
            newKey,
            _entries[newKey]
        );
        Base::_observable.template notify<OnChanged>();
    }

    template <typename Key, typename Value>
    Connection SingleCache<Key, Value>::subscribeToAdded(
        typename OnAdded<Key, Value>::func callback,
        void*                              subscriber
    )
    {
        return Base::_observable.template on<OnAdded<Key, Value>>(
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
        return Base::_observable.template on<OnUpdated<Key, Value>>(
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
        return Base::_observable.template on<OnRemoved<Key>>(
            callback,
            subscriber
        );
    }

    template <typename Key, typename Value>
    Connection SingleCache<Key, Value>::subscribeToChanged(
        typename OnChanged::func callback,
        void*                    subscriber
    )
    {
        return Base::_observable.template on<OnChanged>(callback, subscriber);
    }

    template <typename Key, typename Value>
    const IdMap<Key, std::shared_ptr<const Value>>& SingleCache<Key, Value>::
        _getEntries() const
    {
        return _entries;
    }
}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__SINGLE_CACHE_TPP__
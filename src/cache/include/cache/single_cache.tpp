#ifndef __CACHE__INCLUDE__CACHE__SINGLE_CACHE_TPP__
#define __CACHE__INCLUDE__CACHE__SINGLE_CACHE_TPP__

#include <format>

#include "single_cache.hpp"

namespace cache
{

    /**
     * @brief Get a value from the cache by its key, if the value is not
     * present in the cache, it will be loaded using the _load function and
     * added to the cache.
     *
     * @param key The key of the value to retrieve from the cache.
     * @return std::shared_ptr<const Value> A shared pointer to the cached
     * value, or nullptr if the value could not be loaded.
     */
    template <typename Key, typename Value>
    std::shared_ptr<const Value> SingleCache<Key, Value>::get(const Key& key)
    {
        if (const auto& entry = find(key))
            return entry;

        auto value = _load(key);
        _add(key, value);
        return value;
    }

    /**
     * @brief Find a value in the cache by its key, if the value is not present
     * in the cache, nullptr will be returned.
     *
     * @param key The key of the value to find in the cache.
     * @return std::shared_ptr<const Value> A shared pointer to the cached
     * value, or nullptr if the value is not present in the cache.
     */
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

    /**
     * @brief Get a value from the cache using custom find and load functions,
     * if the value is not present in the cache, it will be loaded using the
     * loadFunc and added to the cache.
     *
     * @param findFunc A function that attempts to find the value in the cache.
     * @param loadFunc A function that loads the value if it is not found in
     * the cache.
     * @return std::shared_ptr<const Value> A shared pointer to the cached
     * value, or nullptr if the value could not be loaded.
     */
    template <typename Key, typename Value>
    std::shared_ptr<const Value> SingleCache<Key, Value>::get(
        std::function<std::shared_ptr<const Value>()> findFunc,
        std::function<std::shared_ptr<const Value>()> loadFunc
    )
    {
        if (const auto& entry = find(findFunc))
            return entry;

        auto value = loadFunc();

        if (value == nullptr)
            return nullptr;

        const auto key = value->getId();
        _add(key, value);
        return value;
    }

    /**
     * @brief Find a value in the cache using a custom find function, if the
     * value is not present in the cache, nullptr will be returned.
     *
     * @param findFunc A function that attempts to find the value in the cache.
     * @return std::shared_ptr<const Value> A shared pointer to the cached
     * value, or nullptr if the value is not present in the cache.
     */
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

    /**
     * @brief Get multiple values from the cache using a custom function that
     * returns a map of key-value pairs, if the values are not present in the
     * cache, they will be loaded and added to the cache.
     *
     * @param func A function that returns a map of key-value pairs to be
     * retrieved from the cache.
     * @return IdObjectMap<std::shared_ptr<const Value>> A map of key-value
     * pairs representing the cached values.
     */
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

    /**
     * @brief Invalidate a specific key in the cache, removing it and its
     * associated value.
     *
     * @param key The key of the value to invalidate in the cache.
     */
    template <typename Key, typename Value>
    void SingleCache<Key, Value>::invalidate(const Key& key)
    {
        this->_recordEviction();
        _entries.removeUnchecked(key);
    }

    /**
     * @brief Clear all keys in the cache, removing all cached values.
     *
     */
    template <typename Key, typename Value>
    void SingleCache<Key, Value>::clear()
    {
        this->_recordEviction();
        _fullyLoaded = false;
        _entries.clear();
    }

    /**
     * @brief Get the current size of the cache, representing the number of
     * cached key-value pairs.
     *
     * @return std::size_t The number of cached key-value pairs in the cache.
     */
    template <typename Key, typename Value>
    std::size_t SingleCache<Key, Value>::size() const
    {
        return _entries.size();
    }

    /**
     * @brief Add a key-value pair to the cache and notify subscribers of the
     * addition.
     *
     * @param key The key of the value to add to the cache.
     * @param value A shared pointer to the value to add to the cache.
     */
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

    /**
     * @brief Add a key-value pair to the cache, if the maximum capacity has not
     * been reached.
     *
     * @tparam Key
     * @tparam Value
     * @param key
     * @param value
     */
    template <typename Key, typename Value>
    void SingleCache<Key, Value>::_add(
        const Key&                   key,
        std::shared_ptr<const Value> value
    )
    {
        bool added = false;
        if (!Base::_maxCapacityReached())
        {
            _entries[key] = value;
            added         = true;
        }

        _fullyLoaded &= added;

        if (added)
            _onAdded(key, value);
    }

    /**
     * @brief Update a key-value pair in the cache, if the key already exists,
     * otherwise add the key-value pair to the cache.
     *
     * @param key The key of the value to update in the cache.
     * @param value A shared pointer to the new value to update in the cache.
     */
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
            _addAndNotify(key, value);
        }
    }

    /**
     * @brief Remove a key-value pair from the cache and notify subscribers of
     * the removal.
     *
     * @param key The key of the value to remove from the cache.
     */
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

    /**
     * @brief Change the key of a value in the cache, if the old key exists and
     * the new key does not exist, otherwise do nothing.
     *
     * @param oldKey The old key of the value to change in the cache.
     * @param newKey The new key of the value to change in the cache.
     */
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
                    "Invalid ID change from {} to {}. Old ID must be negative "
                    "and new ID must be positive.",
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

    /**
     * @brief Subscribe to the OnAdded event of the cache
     *
     * @tparam Key
     * @tparam Value
     * @param callback
     * @param subscriber
     * @return Connection
     */
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

    /**
     * @brief Subscribe to the OnUpdated event of the cache
     *
     * @tparam Key
     * @tparam Value
     * @param callback
     * @param subscriber
     * @return Connection
     */
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

    /**
     * @brief Subscribe to the OnRemoved event of the cache
     *
     * @tparam Key
     * @tparam Value
     * @param callback
     * @param subscriber
     * @return Connection
     */
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

    /**
     * @brief Subscribe to the OnChanged event of the cache
     *
     * @tparam Key
     * @tparam Value
     * @param callback
     * @param subscriber
     * @return Connection
     */
    template <typename Key, typename Value>
    Connection SingleCache<Key, Value>::subscribeToChanged(
        typename OnChanged::func callback,
        void*                    subscriber
    )
    {
        return Base::_observable.template on<OnChanged>(callback, subscriber);
    }

    /**
     * @brief Get the current entries in the cache, representing the cached
     * key-value pairs.
     *
     * @return const IdMap<Key, std::shared_ptr<const Value>>& A reference to
     * the map of cached key-value pairs.
     */
    template <typename Key, typename Value>
    const IdMap<Key, std::shared_ptr<const Value>>& SingleCache<Key, Value>::
        _getEntries() const
    {
        return _entries;
    }
}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__SINGLE_CACHE_TPP__
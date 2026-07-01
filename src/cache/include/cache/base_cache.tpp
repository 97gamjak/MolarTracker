#ifndef __CACHE__INCLUDE__CACHE__BASE_CACHE_TPP__
#define __CACHE__INCLUDE__CACHE__BASE_CACHE_TPP__

#include "cache/base_cache.hpp"

namespace cache
{
    /**
     * @brief Get the current cache statistics, including hits, misses, and
     * evictions.
     *
     * @return const CacheStats&
     */
    template <typename Key, typename Value>
    auto BaseCache<Key, Value>::stats() const -> const CacheStats&
    {
        return _stats;
    }

    /**
     * @brief Reset the cache statistics, clearing hits, misses, and evictions.
     *
     */
    template <typename Key, typename Value>
    void BaseCache<Key, Value>::resetStats()
    {
        _stats = {};
    }

    /**
     * @brief Record a cache hit, incrementing the hit count.
     *
     */
    template <typename Key, typename Value>
    void BaseCache<Key, Value>::_recordHit()
    {
        ++_stats.hits;
    }

    /**
     * @brief Record a cache miss, incrementing the miss count.
     *
     */
    template <typename Key, typename Value>
    void BaseCache<Key, Value>::_recordMiss()
    {
        ++_stats.misses;
    }

    /**
     * @brief Record a cache eviction, incrementing the eviction count.
     *
     */
    template <typename Key, typename Value>
    void BaseCache<Key, Value>::_recordEviction()
    {
        ++_stats.evictions;
    }

    /**
     * @brief Add a connection to the cache, allowing for event notifications.
     *
     * @param connection The connection to be added.
     */
    template <typename Key, typename Value>
    void BaseCache<Key, Value>::_addConnection(Connection&& connection)
    {
        _connections.add(std::move(connection));
    }

    /**
     * @brief Update the maximum capacity of the cache, representing the maximum
     * number of cached key-value pairs allowed in the cache.
     *
     * @param newMaxCapacity The new maximum capacity to be set.
     */
    template <typename Key, typename Value>
    void BaseCache<Key, Value>::updateMaxCapacity(std::size_t newMaxCapacity)
    {
        _maxCapacity = newMaxCapacity;
    }

    /**
     * @brief Check if the maximum capacity of the cache has been reached.
     *
     * @return true if the maximum capacity has been reached, false
     * otherwise.
     */
    template <typename Key, typename Value>
    bool BaseCache<Key, Value>::_maxCapacityReached() const
    {
        return _getMaxCapacity() > 0 && size() >= _getMaxCapacity();
    }

    /**
     * @brief Get the maximum capacity of the cache, representing the maximum
     * number of cached key-value pairs allowed in the cache.
     *
     * @return std::size_t The maximum capacity of the cache.
     */
    template <typename Key, typename Value>
    std::size_t BaseCache<Key, Value>::_getMaxCapacity() const
    {
        return _maxCapacity;
    }

}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__BASE_CACHE_TPP__
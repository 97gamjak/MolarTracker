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
    template <typename Tag, typename Rep>
    void BaseCache<Tag, Rep>::resetStats()
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

}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__BASE_CACHE_TPP__
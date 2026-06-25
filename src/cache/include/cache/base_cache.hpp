#ifndef __CACHE__INCLUDE__CACHE__BASE_CACHE_HPP__
#define __CACHE__INCLUDE__CACHE__BASE_CACHE_HPP__

#include <functional>
#include <memory>

#include "connections/connection.hpp"
#include "connections/observable.hpp"
#include "utils/container/id_map.hpp"

namespace cache
{
    /**
     * @brief Struct representing a callback for when an item is added to the
     * cache.
     *
     * @tparam Key The type of the key used to identify cached values.
     * @tparam Value The type of the value being cached.
     */
    template <typename Key, typename Value>
    struct OnAdded
    {
        using func = std::function<void(const Key& key, const Value* value)>;
    };

    /**
     * @brief Struct representing a callback for when an item is updated in the
     * cache.
     *
     * @tparam Key The type of the key used to identify cached values.
     * @tparam Value The type of the value being cached.
     */
    template <typename Key, typename Value>
    struct OnUpdated
    {
        using func = std::function<void(const Key& key, const Value* value)>;
    };

    /**
     * @brief Struct representing a callback for when an item is removed from
     * the cache.
     *
     * @tparam Key The type of the key used to identify cached values.
     */
    template <typename Key>
    struct OnRemoved
    {
        using func = std::function<void(const Key& key)>;
    };

    /**
     * @brief Struct representing cache statistics, including hits, misses, and
     * evictions.
     *
     */
    struct CacheStats
    {
        std::size_t hits      = 0;
        std::size_t misses    = 0;
        std::size_t evictions = 0;

        [[nodiscard]] double hitRate() const
        {
            auto total = hits + misses;
            return total > 0
                       ? static_cast<double>(hits) / static_cast<double>(total)
                       : 0.0;
        }
    };

    /**
     * @brief Base class for caches, providing common functionality for caching
     * key-value pairs.
     *
     * @tparam Key The type of the key used to identify cached values.
     * @tparam Value The type of the value being cached.
     */
    template <typename Key, typename Value>
    class BaseCache
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        Observable<OnAdded<Key, Value>, OnUpdated<Key, Value>, OnRemoved<Key>>
            _observable;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

       private:
        CacheStats _stats;

        Connections _connections;

       public:
        virtual ~BaseCache() = default;

        [[nodiscard]]
        virtual std::shared_ptr<const Value> get(const Key& key) = 0;

        [[nodiscard]]
        virtual IdObjectMap<std::shared_ptr<const Value>> getAll() = 0;

        virtual void invalidate(const Key& key) = 0;
        virtual void clear()                    = 0;

        [[nodiscard]]
        virtual std::size_t size() const = 0;

        [[nodiscard]] const CacheStats& stats() const { return _stats; }
        void                            resetStats() { _stats = {}; }

       protected:
        void recordHit() { ++_stats.hits; }
        void recordMiss() { ++_stats.misses; }
        void recordEviction() { ++_stats.evictions; }

        [[nodiscard]]
        virtual bool maxCapacityReached() const = 0;

        // TODO:
        void addConnection(Connection&& connection)
        {
            _connections.add(std::move(connection));
        }
    };

}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__BASE_CACHE_HPP__
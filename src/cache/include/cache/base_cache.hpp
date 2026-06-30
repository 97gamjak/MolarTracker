#ifndef __CACHE__INCLUDE__CACHE__BASE_CACHE_HPP__
#define __CACHE__INCLUDE__CACHE__BASE_CACHE_HPP__

#include <functional>
#include <memory>

#include "connections/connection.hpp"
#include "connections/observable.hpp"

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
        /// Type alias for the added callback function
        using func = std::function<
            void(const Key& key, const std::shared_ptr<const Value>& value)>;
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
        /// Type alias for the updated callback function
        using func = std::function<
            void(const Key& key, const std::shared_ptr<const Value>& value)>;
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
        /// Type alias for the removed callback function
        using func = std::function<void(const Key& key)>;
    };

    /**
     * @brief Struct representing a callback for when an item's key changes in
     * the cache.
     *
     * @tparam Key The type of the key used to identify cached values.
     * @tparam Value The type of the value being cached.
     */
    template <typename Key, typename Value>
    struct OnIdChange
    {
        /// Type alias for the ID change callback function
        using func = std::function<void(
            const Key&                          oldKey,
            const Key&                          newKey,
            const std::shared_ptr<const Value>& value
        )>;
    };

    /**
     * @brief Struct representing a callback for when the cache is changed, this
     * can be used to emit an event when the cache is changed, allowing other
     * parts of the application to react to the change.
     *
     */
    struct OnChanged
    {
        /// Type alias for the change callback function
        using func = std::function<void()>;
    };

    /**
     * @brief Struct representing cache statistics, including hits, misses, and
     * evictions.
     *
     */
    struct CacheStats
    {
        /// The number of cache hits
        std::size_t hits = 0;
        /// The number of cache misses
        std::size_t misses = 0;
        /// The number of cache evictions
        std::size_t evictions = 0;

        [[nodiscard]] double hitRate() const;
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
        /// Observable for cache events, allowing for event notifications.
        Observable<
            OnAdded<Key, Value>,
            OnUpdated<Key, Value>,
            OnRemoved<Key>,
            OnIdChange<Key, Value>,
            OnChanged>
            _observable;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

       private:
        /// Cache statistics, including hits, misses, and evictions.
        CacheStats _stats;

        /// Connections for cache events, allowing for event notifications.
        Connections _connections;

        /// The maximum capacity of the cache, representing the maximum number
        /// of cached key-value pairs allowed in the cache.
        std::size_t _maxCapacity = 0;

       public:
        virtual ~BaseCache() = default;

        /**
         * @brief Invalidate a specific key in the cache, removing it and its
         * associated value.
         *
         * @param key
         */
        virtual void invalidate(const Key& key) = 0;

        /**
         * @brief Invalidate all keys in the cache, clearing all cached values.
         *
         */
        virtual void clear() = 0;

        /**
         * @brief Get the current size of the cache, representing the number of
         * cached key-value pairs.
         *
         * @return std::size_t
         */
        [[nodiscard]]
        virtual std::size_t size() const = 0;

        [[nodiscard]] const CacheStats& stats() const;
        void                            resetStats();

        void updateMaxCapacity(std::size_t newMaxCapacity);

       protected:
        void _recordHit();
        void _recordMiss();
        void _recordEviction();

        void _addConnection(Connection&& connection);

        [[nodiscard]] bool _maxCapacityReached() const;

        [[nodiscard]] virtual std::size_t _getMaxCapacity() const;
    };

}   // namespace cache

#ifndef __CACHE__INCLUDE__CACHE__BASE_CACHE_TPP__
#include "base_cache.tpp"
#endif   // __CACHE__INCLUDE__CACHE__BASE_CACHE_TPP__

#endif   // __CACHE__INCLUDE__CACHE__BASE_CACHE_HPP__
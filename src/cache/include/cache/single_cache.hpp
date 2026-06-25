#ifndef __CACHE__INCLUDE__CACHE__SINGLE_CACHE_HPP__
#define __CACHE__INCLUDE__CACHE__SINGLE_CACHE_HPP__

#include <memory>

#include "base_cache.hpp"
#include "connections/observable.hpp"
#include "utils/container/id_map.hpp"

namespace cache
{
    /**
     * @brief A cache that stores a single value for each key, this class is
     * responsible for managing the cache of values associated with keys,
     * allowing for efficient retrieval and management of cached data. It
     * provides methods for getting, invalidating, and clearing cached values,
     * as well as tracking cache statistics such as hits, misses, and evictions.
     *
     * @tparam Key The type of the key used to identify cached values.
     * @tparam Value The type of the value being cached.
     */
    template <typename Key, typename Value>
    class SingleCache : public BaseCache<Key, Value>
    {
       private:
        IdMap<Key, std::shared_ptr<const Value>> _entries;

        using Base = BaseCache<Key, Value>;

        bool _fullyLoaded = false;

       public:
        [[nodiscard]]
        std::shared_ptr<const Value> get(const Key& key) override;

        [[nodiscard]]
        IdObjectMap<std::shared_ptr<const Value>> getAll() override;

        void invalidate(const Key& key) override;

        void clear() override;

        [[nodiscard]] std::size_t size() const override;

        Connection subscribeToAdded(
            typename OnAdded<Key, Value>::func callback,
            void*                              subscriber
        );
        Connection subscribeToUpdated(
            typename OnUpdated<Key, Value>::func callback,
            void*                                subscriber
        );
        Connection subscribeToRemoved(
            typename OnRemoved<Key>::func callback,
            void*                         subscriber
        );

       protected:
        // Derived class implements this — knows how to query the store reader
        [[nodiscard]]
        virtual std::shared_ptr<const Value> _load(const Key& key) = 0;

        [[nodiscard]] virtual IdObjectMap<std::shared_ptr<const Value>> _loadAll(
        ) = 0;

        [[nodiscard]] bool maxCapacityReached() const override = 0;

        // Derived class calls these when wiring store signals in constructor
        void _onStoreAdded(const Key& key, std::shared_ptr<const Value> value);

        void _onStoreUpdated(
            const Key&                   key,
            std::shared_ptr<const Value> value
        );

        void _onStoreRemoved(const Key& key);
    };

}   // namespace cache

#ifndef __CACHE__INCLUDE__CACHE__SINGLE_CACHE_TPP__
#include "single_cache.tpp"
#endif

#endif   // __CACHE__INCLUDE__CACHE__SINGLE_CACHE_HPP__
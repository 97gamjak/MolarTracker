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
        /// the map of cached key-value pairs, where the key is associated with
        /// a shared pointer to the cached value.
        IdMap<Key, std::shared_ptr<const Value>> _entries;

        /// Flag indicating whether the cache has been fully loaded, used to
        /// determine if the cache has reached its maximum capacity and all
        /// possible key-value pairs have been loaded into the cache.
        bool _fullyLoaded = false;

        /// Type alias for the base cache class
        using Base = BaseCache<Key, Value>;

       public:
        [[nodiscard]]
        std::shared_ptr<const Value> get(const Key& key);
        [[nodiscard]]
        std::shared_ptr<const Value> find(const Key& key);

        [[nodiscard]]
        std::shared_ptr<const Value> get(
            std::function<std::shared_ptr<const Value>()> findFunc,
            std::function<std::shared_ptr<const Value>()> loadFunc
        );
        [[nodiscard]]
        std::shared_ptr<const Value> find(
            std::function<std::shared_ptr<const Value>()> findFunc
        );

        [[nodiscard]]
        IdObjectMap<std::shared_ptr<const Value>> getBulk(
            std::function<IdObjectMap<std::shared_ptr<const Value>>()> func
        );

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
        Connection subscribeToChanged(
            typename OnChanged::func callback,
            void*                    subscriber
        );

       protected:
        /**
         * @brief Load a value for the given key, this method should be
         * implemented by derived classes to provide the logic for loading a
         * value for a specific key, such as fetching it from a database or an
         * external service. If the key does not exist, it should return a
         * nullptr.
         *
         * @param key
         * @return std::shared_ptr<const Value>
         */
        [[nodiscard]]
        virtual std::shared_ptr<const Value> _load(const Key& key) = 0;

        /**
         * @brief Handle the event when a value is added to the cache, this
         * method should be implemented by derived classes to provide custom
         * logic for handling the addition of a new key-value pair to the cache,
         * such as logging or triggering other actions.
         *
         * @param key The key of the value that was added to the cache.
         * @param value A shared pointer to the value that was added to the
         * cache.
         */
        virtual void _onAdded(
            const Key&                          key,
            const std::shared_ptr<const Value>& value
        ) = 0;

        /**
         * @brief Handle the event when a value is updated in the cache, this
         * method should be implemented by derived classes to provide custom
         * logic for handling the update of an existing key-value pair in the
         * cache, such as logging or triggering other actions.
         *
         * @param key The key of the value that was updated in the cache.
         * @param oldValue A shared pointer to the old value that was replaced
         * in the cache.
         * @param value A shared pointer to the new value that was added to the
         * cache.
         */
        virtual void _onUpdated(
            const Key&                          key,
            const std::shared_ptr<const Value>& oldValue,
            const std::shared_ptr<const Value>& value
        ) = 0;

        /**
         * @brief Handle the event when a value is removed from the cache, this
         * method should be implemented by derived classes to provide custom
         * logic for handling the removal of a key-value pair from the cache,
         * such as logging or triggering other actions.
         *
         * @param key The key of the value that was removed from the cache.
         */
        virtual void _onRemoved(const Key& key) = 0;

        /**
         * @brief Handle the event when a value's key is changed in the cache,
         * this method should be implemented by derived classes to provide
         * custom logic for handling the change of a key for an existing
         * key-value pair in the cache, such as logging or triggering other
         * actions.
         *
         * @param old The old key of the value that was changed in the cache.
         * @param newKey The new key of the value that was changed in the cache.
         */
        virtual void _onIdChanged(const Key& old, const Key& newKey) = 0;

        [[nodiscard]]
        const IdMap<Key, std::shared_ptr<const Value>>& _getEntries() const;
        void _add(const Key& key, std::shared_ptr<const Value> value);
        void _addAndNotify(const Key& key, std::shared_ptr<const Value> value);
        void _update(const Key& key, std::shared_ptr<const Value> value);
        void _remove(const Key& key);
        void _changeId(const Key& oldKey, const Key& newKey);
    };

}   // namespace cache

#ifndef __CACHE__INCLUDE__CACHE__SINGLE_CACHE_TPP__
#include "single_cache.tpp"
#endif

#endif   // __CACHE__INCLUDE__CACHE__SINGLE_CACHE_HPP__
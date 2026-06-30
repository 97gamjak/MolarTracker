#ifndef __CACHE__INCLUDE__CACHE__CACHE_CONTAINER_HPP__
#define __CACHE__INCLUDE__CACHE__CACHE_CONTAINER_HPP__

#include <memory>

namespace store
{
    class StoreReaders;
    class StoreContainer;
}   // namespace store

namespace cache
{
    class StockCache;   // forward declaration

    /**
     * @brief Container for managing cache instances, this class is responsible
     * for creating and managing the lifecycle of different cache instances used
     * in the application, such as the StockCache. It provides a centralized
     * location for accessing and managing caches, ensuring that they are
     * properly initialized and cleaned up when no longer needed.
     *
     */
    class CacheContainer
    {
        /// Shared pointer to the StockCache instance managed by this container.
        std::shared_ptr<StockCache> _stockCache;

       public:
        explicit CacheContainer(const store::StoreContainer& storeContainer);
        ~CacheContainer();

        [[nodiscard]] const std::shared_ptr<StockCache>& getStockCache() const;
    };
}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__CACHE_CONTAINER_HPP__
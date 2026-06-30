#include "cache/cache_container.hpp"

#include <memory>

#include "cache/stock_cache.hpp"
#include "store/store_container.hpp"

namespace cache
{
    /**
     * @brief Construct a new Cache Container:: Cache Container object
     *
     * @param storeContainer
     */
    CacheContainer::CacheContainer(const store::StoreContainer& storeContainer)
        : _stockCache(
              std::make_shared<StockCache>(storeContainer.getStockStoreReader())
          )
    {
    }

    CacheContainer::~CacheContainer() = default;

    /**
     * @brief Get the StockCache instance managed by this container
     *
     * @return const std::shared_ptr<StockCache>&
     */
    const std::shared_ptr<StockCache>& CacheContainer::getStockCache() const
    {
        return _stockCache;
    }

}   // namespace cache
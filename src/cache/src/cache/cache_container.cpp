#include "cache/cache_container.hpp"

#include <memory>

#include "cache/stock_cache.hpp"
#include "store/store_container.hpp"

namespace cache
{
    CacheContainer::CacheContainer(const store::StoreContainer& storeContainer)
        : _stockCache(
              std::make_shared<StockCache>(storeContainer.getStockStoreReader())
          )
    {
        // empty constructor body
    }

    CacheContainer::~CacheContainer() = default;

}   // namespace cache
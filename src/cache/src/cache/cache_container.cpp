#include "cache/cache_container.hpp"

#include <memory>

#include "cache/account_cache.hpp"
#include "cache/stock_cache.hpp"
#include "settings/cache_settings.hpp"
#include "store/store_container.hpp"

namespace cache
{
    /**
     * @brief Construct a new Cache Container:: Cache Container object
     *
     * @param settings
     * @param storeContainer
     */
    CacheContainer::CacheContainer(
        settings::CacheSettings&     settings,
        const store::StoreContainer& storeContainer
    )
        : _stockCache(
              std::make_shared<StockCache>(storeContainer.getStockStoreReader())
          ),
          _accountCache(
              std::make_shared<AccountCache>(
                  storeContainer.getAccountStoreReader()
              )
          ),
          _connections(std::make_unique<Connections>())
    {
        auto& maxCacheSize = settings.getGlobalMaxCacheSize();
        _connections->add(maxCacheSize.subscribe(
            [this](const auto& newSize)
            {
                _stockCache->updateMaxCapacity(newSize);
                _accountCache->updateMaxCapacity(newSize);
            },
            _stockCache.get()
        )

        );
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

    /**
     * @brief Get the AccountCache instance managed by this container
     *
     * @return const std::shared_ptr<AccountCache>&
     */
    const std::shared_ptr<AccountCache>& CacheContainer::getAccountCache() const
    {
        return _accountCache;
    }

}   // namespace cache
#ifndef __CACHE__INCLUDE__CACHE__CACHE_CONTAINER_HPP__
#define __CACHE__INCLUDE__CACHE__CACHE_CONTAINER_HPP__

#include <memory>

namespace store
{
    class StoreContainer;
}   // namespace store

namespace settings
{
    class CacheSettings;
}   // namespace settings

class Connections;

namespace cache
{
    class StockCache;         // forward declaration
    class AccountCache;       // forward declaration
    class PositionCache;      // forward declaration
    class TransactionCache;   // forward declaration

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

        /// Shared pointer to the AccountCache instance managed by this
        /// container.
        std::shared_ptr<AccountCache> _accountCache;

        std::shared_ptr<PositionCache> _positionCache;

        std::shared_ptr<TransactionCache> _transactionCache;

        /// Connections for cache events, allowing for event notifications.
        std::unique_ptr<Connections> _connections;

       public:
        explicit CacheContainer(
            settings::CacheSettings&     settings,
            const store::StoreContainer& storeContainer
        );
        ~CacheContainer();

        [[nodiscard]]
        const std::shared_ptr<StockCache>& getStockCache() const;

        [[nodiscard]]
        const std::shared_ptr<AccountCache>& getAccountCache() const;

        [[nodiscard]]
        const std::shared_ptr<PositionCache>& getPositionCache() const;

        [[nodiscard]]
        const std::shared_ptr<TransactionCache>& getTransactionCache() const;
    };
}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__CACHE_CONTAINER_HPP__
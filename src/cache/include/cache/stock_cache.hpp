#ifndef __CACHE__INCLUDE__CACHE__STOCK_CACHE_HPP__
#define __CACHE__INCLUDE__CACHE__STOCK_CACHE_HPP__

#include <memory>

#include "config/id_types.hpp"
#include "finance/instrument/stock.hpp"
#include "finance/instrument/stocks.hpp"
#include "single_cache.hpp"
#include "utils/container/id_id_map.hpp"
#include "utils/container/set.hpp"

namespace store
{
    class IStockStoreReader;   // forward declaration
}   // namespace store

namespace finance
{
    struct StockFilter;   // forward declaration
}   // namespace finance

namespace cache
{
    /**
     * @brief A cache for storing stocks, this class is responsible for managing
     * the cache of stock objects, allowing for efficient retrieval and
     * management of cached stock data. It provides methods for getting stocks
     * by their identifiers, as well as tracking cache statistics such as hits,
     * misses, and evictions.
     *
     */
    class StockCache : public SingleCache<StockId, finance::Stock>
    {
       private:
        /// Shared pointer to the stock store reader
        std::shared_ptr<store::IStockStoreReader> _reader;

        /// Map of instrument IDs to stock IDs, used for efficient lookup of
        /// stocks by their associated instrument IDs.
        IdIdMap<InstrumentId, StockId> _instrumentIdMap;

       public:
        explicit StockCache(
            const std::shared_ptr<store::IStockStoreReader>& reader
        );

        [[nodiscard]]
        finance::StocksView getAllStocks();

        [[nodiscard]]
        finance::StocksView getStocks(const IdSet<InstrumentId>& ids);

        [[nodiscard]]
        std::shared_ptr<const finance::Stock> getStock(
            InstrumentId instrumentId
        );

        [[nodiscard]]
        std::shared_ptr<const finance::Stock> getStock(
            const std::string& ticker
        );

       protected:
        [[nodiscard]]
        std::shared_ptr<const finance::Stock> _load(
            const StockId& key
        ) override;

        [[nodiscard]]
        IdObjectMap<std::shared_ptr<const finance::Stock>> _loadAll(
            const finance::StockFilter& filter
        );

        void _onAdded(
            const StockId&                               key,
            const std::shared_ptr<const finance::Stock>& value
        ) override;

        void _onUpdated(
            const StockId&                               key,
            const std::shared_ptr<const finance::Stock>& oldValue,
            const std::shared_ptr<const finance::Stock>& newValue
        ) override;

        void _onRemoved(const StockId& key) override;

        void _onIdChanged(
            const StockId& oldKey,
            const StockId& newKey
        ) override;

       private:
        void _onStoreCommit(
            const IdIdMap<StockId>&      stockIdMap,
            const IdIdMap<InstrumentId>& instrumentIdMap
        );

        [[nodiscard]]
        std::shared_ptr<const finance::Stock> _find(InstrumentId instrumentId);

        [[nodiscard]]
        std::shared_ptr<const finance::Stock> _load(InstrumentId instrumentId);
    };

}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__STOCK_CACHE_HPP__
#ifndef __CACHE__INCLUDE__CACHE__STOCK_CACHE_HPP__
#define __CACHE__INCLUDE__CACHE__STOCK_CACHE_HPP__

#include <memory>

#include "config/id_types.hpp"
#include "finance/instrument/stock.hpp"
#include "finance/instrument/stocks.hpp"
#include "single_cache.hpp"

namespace store
{
    class IStockStoreReader;   // forward declaration
}   // namespace store

namespace cache
{
    class StockCache : public SingleCache<StockId, finance::Stock>
    {
       private:
        std::shared_ptr<store::IStockStoreReader> _reader;

       public:
        explicit StockCache(
            const std::shared_ptr<store::IStockStoreReader>& reader
        );

        [[nodiscard]] finance::StocksView getStocks();

       protected:
        [[nodiscard]] bool maxCapacityReached() const override;

        [[nodiscard]]
        std::shared_ptr<const finance::Stock> _load(
            const StockId& key
        ) override;

        [[nodiscard]]
        IdObjectMap<std::shared_ptr<const finance::Stock>> _loadAll() override;
    };

}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__STOCK_CACHE_HPP__
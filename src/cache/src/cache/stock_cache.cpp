#include "cache/stock_cache.hpp"

#include <memory>
#include <unordered_map>

#include "finance/instrument/stocks.hpp"
#include "store/i_stock_store.hpp"

namespace cache
{
    StockCache::StockCache(
        const std::shared_ptr<store::IStockStoreReader>& reader
    )
        : _reader(reader)
    {
        addConnection(_reader->subscribeToStockAdded(
            [this](const auto& stock)
            {
                _onStoreAdded(
                    stock.getId(),
                    std::make_shared<const finance::Stock>(stock)
                );
            },
            this
        ));
        addConnection(_reader->subscribeToStockUpdated(
            [this](const auto& stock)
            {
                _onStoreUpdated(
                    stock.getId(),
                    std::make_shared<const finance::Stock>(stock)
                );
            },
            this
        ));
        addConnection(_reader->subscribeToStockRemoved(
            [this](const auto& stockId) { _onStoreRemoved(stockId); },
            this
        ));
    }

    bool StockCache::maxCapacityReached() const
    {
        // has no max capacity for now TODO:
        return false;
    }

    std::shared_ptr<const finance::Stock> StockCache::_load(const StockId& key)
    {
        const auto stock = _reader->getStock(key);

        if (stock)
            return std::make_shared<const finance::Stock>(stock.value());

        return nullptr;
    }

    IdObjectMap<std::shared_ptr<const finance::Stock>> StockCache::_loadAll()
    {
        IdObjectMap<std::shared_ptr<const finance::Stock>> result;
        for (const auto& [id, stock] : _reader->getStocks())
            result.addUnchecked(std::make_shared<const finance::Stock>(stock));

        return result;
    }

    finance::StocksView StockCache::getStocks() { return getAll().getItems(); }
}   // namespace cache
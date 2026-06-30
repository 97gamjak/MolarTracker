#include "cache/stock_cache.hpp"

#include <memory>

#include "finance/instrument/instrument_predicates.hpp"
#include "finance/instrument/stock.hpp"
#include "finance/instrument/stocks.hpp"
#include "logging/log_macros.hpp"
#include "store/i_stock_store.hpp"

REGISTER_LOG_CATEGORY("Cache.StockCache");

namespace cache
{

    void StockCache::_onAdded(
        const StockId&                               key,
        const std::shared_ptr<const finance::Stock>& value
    )
    {
        if (value)
            _instrumentIdMap.addUnchecked(value->getInstrumentId(), key);
    }

    void StockCache::_onUpdated(
        const StockId&                               key,
        const std::shared_ptr<const finance::Stock>& oldValue,
        const std::shared_ptr<const finance::Stock>& value
    )
    {
        if (oldValue)
            _instrumentIdMap.removeUnchecked(oldValue->getInstrumentId());

        if (value)
            _instrumentIdMap.addUnchecked(value->getInstrumentId(), key);
    }

    void StockCache::_onRemoved(const StockId& key)
    {
        _instrumentIdMap.erase_if([&key](const auto& pair)
                                  { return pair.second == key; });
    }

    void StockCache::_onIdChanged(
        const StockId& /*oldKey*/,
        const StockId& newKey
    )
    {
        const auto stock = _getEntries()[newKey];
        _instrumentIdMap.removeUnchecked(stock->getInstrumentId());
        _instrumentIdMap.addUnchecked(stock->getInstrumentId(), newKey);
    }

    StockCache::StockCache(
        const std::shared_ptr<store::IStockStoreReader>& reader
    )
        : _reader(reader)
    {
        _addConnection(_reader->subscribeToStockAdded(
            [this](const auto& stock)
            {
                _addAndNotify(
                    stock.getId(),
                    std::make_shared<const finance::Stock>(stock)
                );
            },
            this
        ));
        _addConnection(_reader->subscribeToStockUpdated(
            [this](const auto& stock)
            {
                _update(
                    stock.getId(),
                    std::make_shared<const finance::Stock>(stock)
                );
            },
            this
        ));
        _addConnection(_reader->subscribeToStockRemoved(
            [this](const auto& stockId) { _remove(stockId); },
            this
        ));

        _addConnection(_reader->subscribeToCommit(
            [this](const auto& stockIdMap, const auto& instrumentIdMap)
            { _onStoreCommit(stockIdMap, instrumentIdMap); },
            this
        ));
    }

    bool StockCache::_maxCapacityReached() const
    {
        // has no max capacity for now TODO:
        return false;
    }

    std::shared_ptr<const finance::Stock> StockCache::_load(const StockId& key)
    {
        const auto stock = _reader->getStock(key);

        if (stock)
        {
            return std::make_shared<const finance::Stock>(stock.value());
        }

        return nullptr;
    }

    IdObjectMap<std::shared_ptr<const finance::Stock>> StockCache::_loadAll(
        const finance::StockFilter& filter
    )
    {
        IdObjectMap<std::shared_ptr<const finance::Stock>> result;
        for (const auto& [id, stock] : _reader->getStocks(filter))
        {
            result.addUnchecked(std::make_shared<const finance::Stock>(stock));
        }

        return result;
    }

    finance::StocksView StockCache::getAllStocks()
    {
        return getBulk([this]() { return _loadAll({}); }).getItems();
    }

    finance::StocksView StockCache::getStocks(const IdSet<InstrumentId>& ids)
    {
        finance::StocksView stocks;

        IdSet<InstrumentId> notFound;

        for (const auto& instrumentId : ids)
        {
            const auto& stock =
                find([instrumentId, this]() { return _find(instrumentId); });

            if (stock)
                stocks.addUnchecked(stock);
            else
                notFound.insert(instrumentId);
        }

        finance::StockFilter filter;
        filter.instrumentIds.combine(notFound);

        const auto& storeStocks = _reader->getStocks(filter);
        for (const auto& [stockId, stock] : storeStocks)
        {
            auto stockPtr = std::make_shared<const finance::Stock>(stock);
            stocks.addUnchecked(stockPtr);
            _add(stockId, stockPtr);
        }

        // TODO: make this error handling better!
        if (notFound != storeStocks.getInstrumentIds())
        {
            for (const auto& id : notFound - storeStocks.getInstrumentIds())
            {
                LOG_ERROR(
                    std::format(
                        "Stock with instrument ID {} not found in store or "
                        "database",
                        id.toString()
                    )
                );
            }
        }

        return stocks;
    }

    void StockCache::_onStoreCommit(
        const IdIdMap<StockId>&      stockIdMap,
        const IdIdMap<InstrumentId>& instrumentIdMap
    )
    {
        // update the cache with the new stock IDs
        for (const auto& [oldStockId, newStockId] : stockIdMap)
        {
            _changeId(oldStockId, newStockId);
        }

        for (const auto& [oldInstrumentId, newInstrumentId] : instrumentIdMap)
        {
            for (const auto& [stockId, stock] : _getEntries())
            {
                if (stock->getInstrumentId() == oldInstrumentId)
                {
                    auto updatedStock =
                        std::make_shared<finance::Stock>(*stock);   // copy

                    updatedStock->setInstrumentId(newInstrumentId);

                    const auto newStock =
                        std::make_shared<const finance::Stock>(
                            std::move(*updatedStock)
                        );

                    _update(stockId, newStock);
                }
            }
        }
    }

    std::shared_ptr<const finance::Stock> StockCache::getStock(
        InstrumentId instrumentId
    )
    {
        return get(
            [&]() { return _find(instrumentId); },
            [&]() { return _load(instrumentId); }
        );
    }

    std::shared_ptr<const finance::Stock> StockCache::getStock(
        const std::string& ticker
    )
    {
        return get(
            [&]()
            {
                for (const auto& [id, stockPtr] : _getEntries())
                    if (stockPtr->getTicker() == ticker)
                        return stockPtr;

                return std::shared_ptr<const finance::Stock>(nullptr);
            },
            [&]()
            {
                const auto& stock = _reader->getStock(ticker);

                if (stock)
                    return std::make_shared<const finance::Stock>(stock.value()
                    );

                return std::shared_ptr<const finance::Stock>(nullptr);
            }
        );
    }

    std::shared_ptr<const finance::Stock> StockCache::_find(
        InstrumentId instrumentId
    )
    {
        if (_instrumentIdMap.contains(instrumentId))
        {
            const auto& stockId = _instrumentIdMap[instrumentId];
            return find(stockId);
        }

        return nullptr;
    }

    std::shared_ptr<const finance::Stock> StockCache::_load(
        InstrumentId instrumentId
    )
    {
        const auto& stock = _reader->getStock(instrumentId);
        if (stock)
            return std::make_shared<const finance::Stock>(stock.value());

        return nullptr;
    }

}   // namespace cache
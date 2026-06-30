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

    /**
     * @brief Handle the event when a value is added to the cache, this method
     * updates the instrument ID map with the new stock's instrument ID and
     * stock ID.
     *
     * @param key The key of the value that was added to the cache.
     * @param value A shared pointer to the value that was added to the cache.
     */
    void StockCache::_onAdded(
        const StockId&                               key,
        const std::shared_ptr<const finance::Stock>& value
    )
    {
        if (value)
            _instrumentIdMap.addUnchecked(value->getInstrumentId(), key);
    }

    /**
     * @brief Handle the event when a value is updated in the cache, this method
     * updates the instrument ID map with the new stock's instrument ID and
     * stock ID, and removes the old stock's instrument ID from the map.
     *
     * @param key The key of the value that was updated in the cache.
     * @param oldValue A shared pointer to the old value that was replaced in
     * the cache.
     * @param value A shared pointer to the new value that was added to the
     * cache.
     */
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

    /**
     * @brief Handle the event when a value is removed from the cache, this
     * method removes the stock's instrument ID from the instrument ID map.
     *
     * @param key The key of the value that was removed from the cache.
     */
    void StockCache::_onRemoved(const StockId& key)
    {
        _instrumentIdMap.erase_if([&key](const auto& pair)
                                  { return pair.second == key; });
    }

    /**
     * @brief Handle the event when a stock's ID is changed, this method updates
     * the instrument ID map with the new stock ID.
     *
     * @param oldKey The old key of the stock.
     * @param newKey The new key of the stock.
     */
    void StockCache::_onIdChanged(
        const StockId& /*oldKey*/,
        const StockId& newKey
    )
    {
        const auto stock = _getEntries()[newKey];
        _instrumentIdMap.removeUnchecked(stock->getInstrumentId());
        _instrumentIdMap.addUnchecked(stock->getInstrumentId(), newKey);
    }

    /**
     * @brief Construct a new Stock Cache:: Stock Cache object
     *
     * @param reader
     */
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

    /**
     * @brief Load a stock from the store using its stock ID, this method
     * retrieves the stock from the store and returns a shared pointer to the
     * stock if it exists, otherwise it returns nullptr.
     *
     * @param key The stock ID of the stock to be loaded from the store.
     * @return std::shared_ptr<const finance::Stock> A shared pointer to the
     * loaded stock, or nullptr if the stock does not exist in the store.
     */
    std::shared_ptr<const finance::Stock> StockCache::_load(const StockId& key)
    {
        const auto stock = _reader->getStock(key);

        if (stock)
        {
            return std::make_shared<const finance::Stock>(stock.value());
        }

        return nullptr;
    }

    /**
     * @brief Load all stocks from the store using a filter, this method
     * retrieves the stocks from the store and returns a map of stock IDs to
     * shared pointers to the stocks.
     *
     * @param filter The filter to apply when loading stocks from the store.
     * @return IdObjectMap<std::shared_ptr<const finance::Stock>> A map of stock
     * IDs to shared pointers to the loaded stocks.
     */
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

    /**
     * @brief Get all stocks from the cache, this method retrieves all stocks
     * currently in the cache and returns a StocksView containing shared
     * pointers to the stocks.
     *
     * @return finance::StocksView
     */
    finance::StocksView StockCache::getAllStocks()
    {
        return getBulk([this]() { return _loadAll({}); }).getItems();
    }

    /**
     * @brief Get stocks from the cache using a set of instrument IDs, this
     * method retrieves the stocks associated with the provided instrument IDs
     * and returns a StocksView containing shared pointers to the stocks.
     *
     * @param ids The set of instrument IDs for which to retrieve stocks.
     * @return finance::StocksView
     */
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

    /**
     * @brief Handle the event when the store commits changes, this method
     * updates the cache with the new stock IDs and instrument IDs, ensuring
     * that the cache remains consistent with the store.
     *
     * @param stockIdMap A map of old stock IDs to new stock IDs.
     * @param instrumentIdMap A map of old instrument IDs to new instrument
     * IDs.
     */
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

    /**
     * @brief Get a stock from the cache using its instrument ID, if the stock
     * is not present in the cache, it will be loaded from the store and added
     * to the cache.
     *
     * @param instrumentId The instrument ID of the stock to retrieve.
     * @return std::shared_ptr<const finance::Stock> A shared pointer to the
     * cached stock, or nullptr if the stock could not be loaded.
     */
    std::shared_ptr<const finance::Stock> StockCache::getStock(
        InstrumentId instrumentId
    )
    {
        return get(
            [&]() { return _find(instrumentId); },
            [&]() { return _load(instrumentId); }
        );
    }

    /**
     * @brief Get a stock from the cache using its ticker symbol, if the stock
     * is not present in the cache, it will be loaded from the store and added
     * to the cache.
     *
     * @param ticker The ticker symbol of the stock to retrieve.
     * @return std::shared_ptr<const finance::Stock> A shared pointer to the
     * cached stock, or nullptr if the stock could not be loaded.
     */
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

    /**
     * @brief Find a stock in the cache using its instrument ID, if the stock
     * is not present in the cache, nullptr will be returned.
     *
     * @param instrumentId The instrument ID of the stock to find.
     * @return std::shared_ptr<const finance::Stock> A shared pointer to the
     * cached stock, or nullptr if the stock is not present in the cache.
     */
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

    /**
     * @brief Load a stock from the store using its instrument ID, this method
     * retrieves the stock from the store and returns a shared pointer to the
     * stock if it exists, otherwise it returns nullptr.
     *
     * @param instrumentId The instrument ID of the stock to be loaded from the
     * store.
     * @return std::shared_ptr<const finance::Stock> A shared pointer to the
     * loaded stock, or nullptr if the stock does not exist in the store.
     */
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
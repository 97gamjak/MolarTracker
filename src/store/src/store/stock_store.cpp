#include "store/stock_store.hpp"

#include <algorithm>
#include <format>
#include <utility>
#include <vector>

#include "exceptions/not_yet_implemented.hpp"
#include "finance/instrument/instrument_predicates.hpp"
#include "finance/instrument/stock.hpp"
#include "logging/log_macros.hpp"
#include "store/base/base_store.hpp"
#include "store/base/store_state.hpp"

REGISTER_LOG_CATEGORY("Store.StockStore");

using finance::Stock;

namespace store
{

    /**
     * @brief Construct a new Stock Store:: Stock Store object
     *
     * @param instrumentService
     * @param instrumentIdSeq
     */
    StockStore::StockStore(
        InstrumentServicePtr instrumentService,
        InstrumentIdSeq&     instrumentIdSeq
    )
        : BaseStore<finance::Stock, StockId>(true),
          _instrumentService(std::move(instrumentService)),
          _instrumentIdSeq(instrumentIdSeq)
    {
        // empty id set returns all stocks
        const auto& stocks = _instrumentService->getStocks({});

        _addCleanEntries(stocks.getValues());
    }

    /**
     * @brief Add a stock to the store, this will check if a stock with the same
     * ticker already exists in the store or in the database, and if not, it
     * will add the stock to the store and the database.
     *
     * @param stock The Stock object to be added to the store
     * @return StockStoreResult indicating the result of the operation
     */
    StockStoreResult StockStore::addStock(Stock stock)
    {
        const auto ticker = stock.getTicker();

        // check if the stock exists in the db but not in store
        if (stockExists(ticker, true))
        {
            LOG_ERROR(
                std::format(
                    "Stock with ticker {} already exists in the database",
                    ticker
                )
            );

            return StockStoreResult::StockAlreadyExists;
        }

        stock.setInstrumentId(_instrumentIdSeq.next());
        _addEntry(std::move(stock));

        return StockStoreResult::Ok;
    }

    /**
     * @brief Check if a stock with the given ticker exists in the store
     *
     * @param ticker The ticker symbol of the stock to check
     * @param checkDeleted Whether to include deleted stocks in the check
     * @return true if the stock exists, false otherwise
     */
    bool StockStore::stockExists(
        const std::string& ticker,
        bool               checkDeleted
    ) const
    {
        const auto options = Options{
            .filter   = finance::HasTicker(ticker),
            .deletion = checkDeleted ? DeletionPolicy::IncludeDelete
                                     : DeletionPolicy::ExcludeDelete
        };

        auto exists = _getEntry(options).has_value();

        if (!isFullCache())
            exists |= _instrumentService->stockExists(ticker);

        return exists;
    }

    /**
     * @brief Commit the changes in the stock store
     *
     */
    void StockStore::commit()
    {
        LOG_ENTRY;

        // make an early return to not notify unnecessarily
        if (!isDirty())
            return;

        _instrumentIdMap.clear();

        for (const auto& entry : _getEntries())
        {
            switch (entry.state)
            {
                case StoreState::New:
                {
                    const auto insertionResult =
                        _instrumentService->addStock(entry.value);

                    LOG_DEBUG(
                        std::format(
                            "Added new stock: {} with ID: {} and Instrument "
                            "ID: {}",
                            entry.value.toString(),
                            insertionResult.stockId.toString(),
                            insertionResult.instrumentId.toString()
                        )
                    );

                    auto stock = entry.value;
                    stock.setId(insertionResult.stockId);
                    stock.setInstrumentId(insertionResult.instrumentId);
                    const auto oldInstrumentId = entry.value.getInstrumentId();

                    const auto result = _commitEntry(
                        entry.value.getId(),
                        Entry{.value = stock, .state = entry.state}
                    );

                    if (result != StoreResult::Ok)
                    {
                        throw std::runtime_error(
                            "Failed to add new stock entry to database"
                        );
                    }

                    if (oldInstrumentId != insertionResult.instrumentId)
                        _instrumentIdMap[oldInstrumentId] =
                            insertionResult.instrumentId;

                    break;
                }
                case StoreState::Modified:
                case StoreState::Deleted:
                {
                    throw NotYetImplementedException(
                        StoreStateMeta::toString(entry.state) +
                        " not yet implemented"
                    );
                }
                case StoreState::Clean:
                {
                    break;
                }
            }
        }
    }

    /**
     * @brief Get a list of all stocks in the store
     *
     * @return finance::Stocks
     */
    finance::Stocks StockStore::getStocks() const { return _getStocks({}); }

    /**
     * @brief Get a list of all stocks in the store
     *
     * @param ids The set of instrument IDs to retrieve stocks for
     * @return finance::Stocks
     */
    finance::Stocks StockStore::getStocks(const IdSet<InstrumentId>& ids) const
    {
        finance::StockFilter filter;
        filter.instrumentIds.combine(ids);

        return _getStocks(filter);
    }

    /**
     * @brief Get a stock by its instrument ID
     *
     * @param id The instrument ID
     * @return std::optional<Stock>
     */
    std::optional<Stock> StockStore::getStock(StockId id) const
    {
        finance::StockFilter filter;
        filter.stockIds.combine({id});

        return _getStock(filter);
    }

    /**
     * @brief Get a stock by its instrument ID
     *
     * @param id The instrument ID
     * @return std::optional<Stock>
     */
    std::optional<Stock> StockStore::getStock(InstrumentId id) const
    {
        finance::StockFilter filter;
        filter.instrumentIds.combine({id});

        return _getStock(filter);
    }

    /**
     * @brief Get a list of all stock tickers in the store
     *
     * @return std::unordered_set<std::string>
     */
    std::unordered_set<std::string> StockStore::getAllTickers() const
    {
        std::vector<std::string> tickers;

        return getStocks().getTickers();
    }

    /**
     * @brief Get a mapping of stock tickers to their instrument IDs
     *
     * @return std::unordered_map<std::string, InstrumentId>
     */
    std::unordered_map<std::string, InstrumentId> StockStore::getTickerMap(
    ) const
    {
        std::unordered_map<std::string, InstrumentId> tickerMap;

        for (const auto& [id, stock] : getStocks())
            tickerMap[stock.getTicker()] = stock.getInstrumentId();

        return tickerMap;
    }

    /**
     * @brief Get a mapping of instrument IDs to their names
     *
     * @return unorderedIdMap<InstrumentId, std::string>
     */
    unorderedIdMap<InstrumentId, std::string> StockStore::
        getInstrumentIdToNameMap() const
    {
        unorderedIdMap<InstrumentId, std::string> map;

        for (const auto& [id, stock] : getStocks())
            map[stock.getInstrumentId()] = stock.getTicker();

        return map;
    }

    /**
     * @brief Get the instrument ID for a given stock ticker
     *
     * @param ticker The stock ticker
     * @return std::optional<InstrumentId>
     */
    std::optional<InstrumentId> StockStore::getInstrumentId(
        const std::string& ticker
    ) const
    {
        for (const auto& [id, stock] : getStocks())
            if (stock.getTicker() == ticker)
                return stock.getInstrumentId();

        return std::nullopt;
    }

    /**
     * @brief Get the mapping of old instrument IDs to new instrument IDs after
     * a commit
     *
     * @return const IdIdMap<InstrumentId>&
     */
    const IdIdMap<InstrumentId>& StockStore::getInstrumentIdMap() const
    {
        return _instrumentIdMap;
    }

    /**
     * @brief Subscribe to changes in the stock store, this will notify the
     * subscriber whenever a stock is added, modified or deleted in the store.
     *
     * @param func The function to be called when a change occurs, this function
     * should take a StockId as a parameter and return void, it will be called
     * with the ID of the stock that was changed.
     * @param subscriber A pointer to the subscriber object, this is used to
     * identify the subscriber and manage the subscription, it can be any
     * pointer (e.g. to a class instance) and is not used by the store itself,
     * but it should be unique for each subscriber to avoid conflicts.
     * @return Connection An object representing the subscription, this can be
     * used to manage the subscription (e.g. to unsubscribe) and should be
     * stored by the subscriber if they want to manage their subscriptions.
     */
    Connection StockStore::subscribeToStoreChange(
        StoreChanged<StockId>::func func,
        void*                       subscriber
    )
    {
        return BaseStore<finance::Stock, StockId>::subscribeToStoreChange(
            func,
            subscriber
        );
    }

    Connection StockStore::subscribeToStockAdded(
        OnStoreItemAdded<Stock>::func func,
        void*                         subscriber
    )
    {
        return BaseStore<finance::Stock, StockId>::subscribeToEntryAdded(
            func,
            subscriber
        );
    }

    Connection StockStore::subscribeToStockUpdated(
        OnStoreItemUpdated<Stock>::func func,
        void*                           subscriber
    )
    {
        return BaseStore<finance::Stock, StockId>::subscribeToEntryUpdated(
            func,
            subscriber
        );
    }

    Connection StockStore::subscribeToStockRemoved(
        OnStoreItemRemoved<StockId>::func func,
        void*                             subscriber
    )
    {
        return BaseStore<finance::Stock, StockId>::subscribeToEntryRemoved(
            func,
            subscriber
        );
    }

    std::optional<Stock> StockStore::_getStock(
        const finance::StockFilter& filter
    ) const
    {
        const auto stocks = _getStocks(filter);

        if (stocks.size() > 1)
            throw std::runtime_error("Multiple stocks found");

        if (stocks.empty())
            return std::nullopt;

        return stocks.begin()->second;
    }

    /**
     * @brief Get a list of all stocks in the store
     *
     * @param filter The filter to apply when retrieving stocks
     * @return finance::Stocks
     */
    finance::Stocks StockStore::_getStocks(
        const finance::StockFilter& filter
    ) const
    {
        auto options = Options{
            .filter   = filter.makePredicates(),
            .deletion = DeletionPolicy::ExcludeDelete,
        };

        auto entries = _getValues(options);

        finance::Stocks stocks;

        for (const auto& entry : entries)
            stocks.addUnchecked(entry);

        if (!isFullCache())
        {
            options.deletion = DeletionPolicy::IncludeDelete;

            for (const auto& [stockId, stock] :
                 _instrumentService->getStocks(filter))
            {
                const auto alreadyInStore = std::ranges::any_of(
                    _getValues(options),
                    [_stockId = stockId](const Stock& stockInStore)
                    { return stockInStore.getId() == _stockId; }
                );

                if (!alreadyInStore)
                    stocks.addUnchecked(stock);
            }
        }

        return stocks;
    }

}   // namespace store
#include "store/stock_store.hpp"

#include <algorithm>
#include <format>
#include <utility>

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
        std::shared_ptr<service::IInstrumentService> instrumentService,
        InstrumentIdSeq&                             instrumentIdSeq
    )
        : _instrumentService(std::move(instrumentService)),
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
        finance::StockFilter filter;
        filter.tickers.combine({ticker});

        const auto options = Options{
            .filter   = filter.makePredicates(),
            .deletion = checkDeleted ? DeletionPolicy::IncludeDelete
                                     : DeletionPolicy::ExcludeDelete
        };

        auto exists = _getEntry(options).has_value();

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

        _notifyCommit();
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

    std::optional<finance::Stock> StockStore::getStock(
        const std::string& ticker
    ) const
    {
        finance::StockFilter filter;
        filter.tickers.combine({ticker});

        return _getStock(filter);
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

        for (const auto& [id, stock] : getStocks({}))
            tickerMap[stock.getTicker()] = stock.getInstrumentId();

        return tickerMap;
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
     * @brief Subscribe to stock added events
     *
     * @param func The callback function to be called when a stock is added
     * @param subscriber The subscriber object that will receive the event
     *
     * @return Connection The connection object for managing the subscription
     */
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

    /**
     * @brief Subscribe to stock updated events
     *
     * @param func The callback function to be called when a stock is updated
     * @param subscriber The subscriber object that will receive the event
     *
     * @return Connection The connection object for managing the subscription
     */
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

    /**
     * @brief Subscribe to stock removed events
     *
     * @param func The callback function to be called when a stock is removed
     * @param subscriber The subscriber object that will receive the event
     *
     * @return Connection The connection object for managing the subscription
     */
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

    /**
     * @brief Get a stock based on the provided filter, this method retrieves
     * stocks from the store that match the given filter criteria. If multiple
     * stocks match the filter, an exception is thrown. If no stocks match, an
     * empty optional is returned.
     *
     * @param filter The filter to apply when retrieving stocks
     * @return std::optional<Stock>
     */
    std::optional<Stock> StockStore::_getStock(
        const finance::StockFilter& filter
    ) const
    {
        const auto stocks = getStocks(filter);

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
    finance::Stocks StockStore::getStocks(
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

        return stocks;
    }

    /**
     * @brief Subscribe to commit events, this allows subscribers to be notified
     * when a commit occurs in the stock store, providing them with the mapping
     * of old instrument IDs to new instrument IDs.
     *
     * @param func The callback function to be called when a commit occurs
     * @param subscriber The subscriber object that will receive the event
     * @return Connection The connection object for managing the subscription
     */
    Connection StockStore::subscribeToCommit(
        const OnCommit::func& func,
        void*                 subscriber
    )
    {
        return _onCommit.on<OnCommit>(func, subscriber);
    }

    /**
     * @brief Notify subscribers of a commit event, this method is called
     * internally when a commit occurs in the stock store, and it notifies all
     * subscribers with the mapping of old instrument IDs to new instrument IDs.
     *
     */
    void StockStore::_notifyCommit()
    {
        _onCommit.notify<OnCommit>(_getIdRemap(), getInstrumentIdMap());
    }

}   // namespace store
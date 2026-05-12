#include "app/store/stock_store.hpp"

#include <algorithm>
#include <utility>
#include <vector>

#include "app/store/base/base_store.hpp"
#include "app/store/base/store_state.hpp"
#include "exceptions/not_yet_implemented.hpp"
#include "finance/instrument/instrument_predicates.hpp"
#include "finance/instrument/stock.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("App.Store.StockStore");

using finance::Stock;

namespace app
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
        : _instrumentService(std::move(instrumentService)),
          _instrumentIdSeq(instrumentIdSeq)
    {
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

        return _getEntry(options).has_value() ||
               _instrumentService->stockExists(ticker);
    }

    /**
     * @brief Get a list of all stocks in the store, this will return all stocks
     * that are not marked as deleted, and will include stocks that are new or
     * modified but not yet saved to the database.
     *
     * @return std::vector<Stock>
     */
    std::vector<Stock> StockStore::getStocks() const
    {
        auto options = Options{.deletion = DeletionPolicy::ExcludeDelete};

        auto entries = _getValues(options);

        std::vector<Stock> stocks;

        for (const auto& entry : entries)
            stocks.push_back(entry);

        for (const auto& stock : _instrumentService->getStocks())
        {
            const auto alreadyInStore = std::ranges::any_of(
                stocks,
                [&](const Stock& stockInStore)
                { return stockInStore.getId() == stock.getId(); }
            );

            if (!alreadyInStore)
                stocks.push_back(stock);
        }

        return stocks;
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

        instrumentMap<InstrumentId> map{};
        for (const auto& entry : _getEntries())
        {
            switch (entry.state)
            {
                case StoreState::New:
                {
                    const auto [stockId, instrumentId] =
                        _instrumentService->addStock(entry.value);

                    LOG_DEBUG(
                        std::format(
                            "Added new stock: {} with ID: {} and Instrument "
                            "ID: {}",
                            entry.value.toString(),
                            stockId.toString(),
                            instrumentId.toString()
                        )
                    );

                    auto stock = entry.value;
                    stock.setId(stockId);
                    stock.setInstrumentId(instrumentId);
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

                    if (oldInstrumentId != instrumentId)
                        map[oldInstrumentId] = instrumentId;

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

        _notifyOnCommit();
        if (!map.empty())
            _onInstrumentIdRemap.notify<OnIdRemap<InstrumentId>>(map);
    }

    /**
     * @brief Get a stock by its instrument ID
     *
     * @param id The instrument ID
     * @return std::optional<Stock>
     */
    std::optional<Stock> StockStore::getStock(InstrumentId id) const
    {
        const auto options = Options{
            .filter   = finance::HasInstrumentId(id),
            .deletion = DeletionPolicy::ExcludeDelete
        };
        auto stocksView = _getValues(options);

        if (stocksView.empty())
        {
            // TODO(97gamjak): Handle case where stock is not found and needs to
            // be searched for in database
            return std::nullopt;
        }

        const std::vector<Stock> stocks = {
            stocksView.begin(),
            stocksView.end()
        };

        if (stocks.size() > 1)
            throw std::runtime_error("Multiple stocks found");

        return stocks.front();
    }

    /**
     * @brief Get a list of all stock tickers in the store
     *
     * @return std::vector<std::string>
     */
    std::vector<std::string> StockStore::getAllTickers() const
    {
        std::vector<std::string> tickers;

        for (const auto& stock : getStocks())
            tickers.push_back(stock.getTicker());

        return tickers;
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

        for (const auto& stock : getStocks())
            tickerMap[stock.getTicker()] = stock.getInstrumentId();

        return tickerMap;
    }

    /**
     * @brief Get a mapping of instrument IDs to their names
     *
     * @return instrumentMap<std::string>
     */
    instrumentMap<std::string> StockStore::getInstrumentIdToNameMap() const
    {
        instrumentMap<std::string> map;

        for (const auto& stock : getStocks())
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
        for (const auto& stock : getStocks())
            if (stock.getTicker() == ticker)
                return stock.getInstrumentId();

        return std::nullopt;
    }

    /**
     * @brief Subscribe to instrument ID remapping events
     *
     * @param func The callback function to be called on remapping
     * @param userData User data to be passed to the callback
     * @return Connection
     */
    Connection StockStore::subscribeToInstrumentIdRemap(
        OnIdRemap<InstrumentId>::func func,
        void*                         userData
    )
    {
        return _onInstrumentIdRemap.on<OnIdRemap<InstrumentId>>(
            std::move(func),
            userData
        );
    }

}   // namespace app
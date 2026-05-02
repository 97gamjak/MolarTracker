#include "app/store/stock_store.hpp"

#include <algorithm>
#include <vector>

#include "app/store/base/store_state.hpp"
#include "exceptions/not_yet_implemented.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("App.Store.StockStore");

namespace app
{

    /**
     * @brief Construct a new Stock Store:: Stock Store object
     *
     * @param instrumentService
     */
    StockStore::StockStore(InstrumentServicePtr instrumentService)
        : _instrumentService(std::move(instrumentService))
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
    StockStoreResult StockStore::addStock(finance::Stock stock)
    {
        const auto ticker = stock.getTicker();

        auto options = Options{
            .filter   = finance::HasTicker(ticker),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        const auto existingStock = _get(options);

        if (existingStock.has_value())
        {
            LOG_ERROR(
                std::format("Stock with ticker {} already exists", ticker)
            );

            return StockStoreResult::StockAlreadyExists;
        }

        const auto stockExistsInDb = _instrumentService->stockExists(ticker);

        options.deletion   = DeletionPolicy::OnlyDeleted;
        auto* deletedStock = _findEntry(options);

        if (stockExistsInDb && deletedStock == nullptr)
        {
            LOG_ERROR(
                std::format(
                    "Stock with ticker {} already exists in the database",
                    ticker
                )
            );

            return StockStoreResult::StockAlreadyExists;
        }

        if (stockExistsInDb && deletedStock != nullptr)
        {
            deletedStock->value = stock;
            deletedStock->state = StoreState::Modified;
        }
        else
        {
            const auto newId = _generateNewId();
            stock.setId(newId);

            _addEntry(stock, StoreState::New);
        }

        return StockStoreResult::Ok;
    }

    /**
     * @brief Get a list of all stocks in the store, this will return all stocks
     * that are not marked as deleted, and will include stocks that are new or
     * modified but not yet saved to the database.
     *
     * @return std::vector<finance::Stock>
     */
    std::vector<finance::Stock> StockStore::getStocks() const
    {
        auto options = Options{.deletion = DeletionPolicy::ExcludeDelete};

        auto entries = _getValues(options);

        std::vector<finance::Stock> stocks;

        for (const auto& entry : entries)
            stocks.push_back(entry);

        for (const auto& stock : _instrumentService->getStocks())
        {
            const auto alreadyInStore = std::ranges::any_of(
                stocks,
                [&](const finance::Stock& stockInStore)
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
        for (auto& entry : _getEntries())
        {
            if (entry.state == StoreState::New)
            {
                const auto [stockId, instrumentId] =
                    _instrumentService->addStock(entry.value);

                entry.value.setId(stockId);
                entry.value.setInstrumentId(instrumentId);
            }
            else if (entry.state == StoreState::Modified)
            {
                throw NotYetImplementedException(
                    "Stock modification is not yet implemented"
                );
            }
        }

        _clearEntries();
    }

    std::vector<std::string> StockStore::getAllTickers() const
    {
        std::vector<std::string> tickers;

        for (const auto& stock : getStocks())
        {
            tickers.push_back(stock.getTicker());
        }

        return tickers;
    }

}   // namespace app
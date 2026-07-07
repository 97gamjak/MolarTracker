#include "cache/stock_cache.hpp"

namespace cache
{
    /**
     * @brief Get a stock from the cache, this method retrieves a stock by its
     * instrument ID from the provided stock cache, performing validation to
     * ensure that the stock exists in the cache.
     *
     * @param instrumentId The instrument ID of the stock to retrieve.
     * @param stockCache A shared pointer to the StockCache instance from which
     * to retrieve the stock.
     *
     * @return Result<std::shared_ptr<const finance::Stock>, FinanceError> A
     * result containing a shared pointer to the stock if found, or a
     * FinanceError indicating the reason for failure (e.g., stock not found).
     */
    Result<std::shared_ptr<const finance::Stock>, FinanceError> StockCacheUtils::
        getStock(
            InstrumentId                       instrumentId,
            const std::shared_ptr<StockCache>& stockCache
        )
    {
        const auto& stock = stockCache->getStock(instrumentId);

        if (stock == nullptr)
        {
            return FinanceError(
                FinanceErrorType::StockNotFound,
                std::format(
                    "Stock with instrument ID {} not found in cache",
                    instrumentId.toString()
                )
            );
        }

        return stock;
    }
}   // namespace cache
#include "cache/stock_cache.hpp"

namespace cache
{
    MTResult<std::shared_ptr<const finance::Stock>, FinanceError> StockCacheUtils::
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
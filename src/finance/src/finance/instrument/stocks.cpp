#include "finance/instrument/stocks.hpp"

#include <unordered_set>

namespace finance
{
    /**
     * @brief Get the set of tickers for all stocks in the collection.
     *
     * @return std::unordered_set<std::string>
     */
    std::unordered_set<std::string> Stocks::getTickers() const
    {
        std::unordered_set<std::string> tickers;
        tickers.reserve(size());

        for (const auto& [id, stock] : *this)
            tickers.insert(stock.getTicker());

        return tickers;
    }
}   // namespace finance
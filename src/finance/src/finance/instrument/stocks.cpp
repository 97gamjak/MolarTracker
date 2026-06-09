#include "finance/instrument/stocks.hpp"

#include <unordered_set>

namespace finance
{
    std::unordered_set<std::string> Stocks::getTickers() const
    {
        std::unordered_set<std::string> tickers;
        tickers.reserve(size());

        for (const auto& [id, stock] : *this)
            tickers.insert(stock.getTicker());

        return tickers;
    }
}   // namespace finance
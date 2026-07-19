#include "finance/instrument/stocks.hpp"

namespace finance
{
    /**
     * @brief Get the set of tickers for all stocks in the collection.
     *
     * @return std::unordered_set<std::string>
     */
    Set<std::string> Stocks::getTickers() const
    {
        Set<std::string> tickers;

        for (const auto& [id, stock] : *this)
            tickers.insert(stock.getTicker());

        return tickers;
    }
}   // namespace finance
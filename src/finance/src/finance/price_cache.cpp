#include "finance/price_cache.hpp"

#include "finance/yf_client.hpp"

namespace finance
{
    /**
     * @brief Updates the price cache with new quotes.
     *
     * @param quotes The new price quotes to add or update.
     */
    void PriceCache::update(
        const std::unordered_map<std::string, PriceQuote>& quotes
    )
    {
        for (const auto& [symbol, quote] : quotes)
            _quotes.insert_or_assign(symbol, quote);
    }

    /**
     * @brief Retrieves a price quote from the cache.
     *
     * @param yahooSymbol The Yahoo Finance symbol to look up.
     * @return The cached price quote, if found.
     */
    std::optional<PriceQuote> PriceCache::get(
        const std::string& yahooSymbol
    ) const
    {
        auto it = _quotes.find(yahooSymbol);

        if (it != _quotes.end())
            return it->second;

        return std::nullopt;
    }

    /**
     * @brief Clears all cached price quotes.
     *
     */
    void PriceCache::clear() { _quotes.clear(); }

    /**
     * @brief Fetches a price quote from Yahoo Finance.
     *
     * @param yahooSymbol The Yahoo Finance symbol to look up.
     * @return The fetched price quote, if successful.
     */
    std::optional<PriceQuote> PriceFeedService::fetchQuote(
        const std::string& yahooSymbol
    )
    {
        auto result = YahooFinanceClient::fetchPrice(yahooSymbol);

        if (!result)
            return std::nullopt;

        return result.value();
    }

    /**
     * @brief Fetches a batch of price quotes from Yahoo Finance.
     *
     * @param yahooSymbols The Yahoo Finance symbols to look up.
     * @return A map of fetched price quotes, indexed by their symbols.
     */
    std::unordered_map<std::string, PriceQuote> PriceFeedService::fetchBatch(
        const std::vector<std::string>& yahooSymbols
    )
    {
        std::unordered_map<std::string, PriceQuote> results;
        results.reserve(yahooSymbols.size());

        for (const auto& symbol : yahooSymbols)
        {
            if (auto quote = fetchQuote(symbol))
                results.emplace(symbol, quote.value());
        }

        return results;
    }

}   // namespace finance

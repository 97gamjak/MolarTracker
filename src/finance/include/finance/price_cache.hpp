#ifndef __FINANCE__INCLUDE__FINANCE__PRICE_CACHE_HPP__
#define __FINANCE__INCLUDE__FINANCE__PRICE_CACHE_HPP__

#include <optional>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "connections/observable.hpp"
#include "price_quote.hpp"

namespace finance
{
    /**
     * @brief Event triggered when price quotes in the cache are updated.
     *
     */
    struct OnPriceUpdated
    {
        /// Callback function type for price updates.
        using func = std::function<void()>;
    };

    /**
     * @brief Caches price quotes for financial instruments.
     *
     */
    class PriceCache : public Observable<OnPriceUpdated>
    {
       private:
        /// Mutex for synchronizing access to the cache.
        mutable std::shared_mutex _mutex;

        /// Maps Yahoo Finance symbols to their price quotes.
        std::unordered_map<std::string, PriceQuote> _quotes;

        /// Set of tickers that have been requested but not yet fetched, used to
        /// gate updates until all expected quotes are received.
        std::unordered_set<std::string> _tickersNotYetFetched;

       public:
        void update(const std::unordered_map<std::string, PriceQuote>& quotes);

        void addTicker(const std::string& yahooSymbol);

        [[nodiscard]]
        std::optional<PriceQuote> get(const std::string& yahooSymbol) const;

        void clear();

        [[nodiscard]]
        Connection subscribeToPriceChange(
            OnPriceUpdated::func callback,
            void*                user
        );
    };

    /**
     * @brief Fetches end-of-day quotes from Yahoo Finance.
     *
     * All methods are static and safe to call from a worker thread
     * (e.g. via QtConcurrent::run). No Qt types are used so that the
     * service stays off the main thread without issue.
     *
     * Each security must carry a yahooSymbol field (e.g. "AAPL", "APC.DE")
     * that is distinct from the display ticker. Mapping that field to the
     * right instrument is the caller's responsibility.
     *
     * Note: Yahoo Finance's v8 endpoint is unofficial and can break without
     * warning. If it does, only the URL construction in fetchQuote() needs
     * to change.
     */
    class PriceFeedService
    {
       public:
        [[nodiscard]]
        static std::optional<PriceQuote> fetchQuote(
            const std::string& yahooSymbol
        );

        [[nodiscard]]
        static std::unordered_map<std::string, PriceQuote> fetchBatch(
            const std::unordered_set<std::string>& yahooSymbols
        );
    };
}   // namespace finance

#endif   //  __FINANCE__INCLUDE__FINANCE__PRICE_CACHE_HPP__
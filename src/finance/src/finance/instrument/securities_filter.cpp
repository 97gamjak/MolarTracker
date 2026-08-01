#include "finance/instrument/securities_filter.hpp"

#include <algorithm>

#include "filter/predicate.hpp"

namespace finance
{
    namespace
    {
        /**
         * @brief Get a predicate function that can be used to filter stocks
         * based on the specified symbols.
         *
         * @param symbols The list of symbols to filter by
         * @return filter::Predicate<Stock> A predicate function that can be
         * used to filter stocks based on the specified symbols
         */
        filter::Predicate<Stock> HasSymbol(const Set<std::string>& symbols)
        {
            return filter::makePredicate<Stock>(
                [symbols](const Stock& stock)
                {
                    return std::ranges::any_of(
                        symbols,
                        [&](const std::string& symbol)
                        { return stock.getTicker() == symbol; }
                    );
                }
            );
        }

        /**
         * @brief Get a predicate function that can be used to filter stocks
         * based on the specified stock IDs.
         *
         * @param stockIds The set of stock IDs to filter by
         * @return filter::Predicate<Stock> A predicate function that can be
         * used to filter stocks based on the specified stock IDs
         */
        filter::Predicate<Stock> HasStockId(const Set<StockId>& stockIds)
        {
            return filter::makePredicate<Stock>(
                [stockIds](const Stock& stock)
                { return stockIds.contains(stock.getId()); }
            );
        }

        /**
         * @brief Get a predicate function that can be used to filter stocks
         * based on the specified instrument IDs.
         *
         * @param instrumentIds The set of instrument IDs to filter by
         * @return filter::Predicate<Stock> A predicate function that can be
         * used to filter stocks based on the specified instrument IDs
         */
        filter::Predicate<Stock> HasInstrumentId(
            const Set<InstrumentId>& instrumentIds
        )
        {
            return filter::makePredicate<Stock>(
                [instrumentIds](const Stock& stock)
                { return instrumentIds.contains(stock.getInstrumentId()); }
            );
        }
    }   // namespace

    /**
     * @brief Get a predicate function that can be used to filter stocks based
     * on the criteria set in this filter, this function generates a predicate
     * that can be applied to a collection of stocks to filter them according to
     * the symbols and stock IDs specified in this filter. If no symbols or
     * stock IDs are set, the returned predicate will not filter out any stocks.
     *
     * @return filter::Predicate<Stock> A predicate function that can be used to
     * filter stocks based on the criteria set in this filter.
     */
    filter::Predicate<Stock> SecuritiesFilter::getStockPredicate() const
    {
        filter::Predicate<Stock> predicate;

        if (symbols.has_value())
            predicate &= HasSymbol(symbols.value());

        if (stockIds.has_value())
            predicate &= HasStockId(stockIds.value());

        if (instrumentIds.has_value())
            predicate &= HasInstrumentId(instrumentIds.value());

        return predicate;
    }

}   // namespace finance

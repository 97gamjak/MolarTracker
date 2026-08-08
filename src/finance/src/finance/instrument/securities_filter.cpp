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
     * @brief Get a string representation of the SecuritiesFilter, this will
     * include the symbols, stock IDs, and instrument IDs that are set in the
     * filter, and will be used for logging and debugging purposes to provide
     * insight into the current state of the filter.
     *
     * @return std::string A string representation of the SecuritiesFilter
     */
    std::string SecuritiesFilter::toString() const
    {
        std::string result = "SecuritiesFilter(";

        if (symbols.has_value())
        {
            result += "symbols: {";
            for (const auto& symbol : symbols.value())
                result += symbol + ", ";
            result += "}, ";
        }

        if (stockIds.has_value())
        {
            result += "stockIds: {";
            for (const auto& stockId : stockIds.value())
                result += std::to_string(stockId.value()) + ", ";
            result += "}, ";
        }

        if (instrumentIds.has_value())
        {
            result += "instrumentIds: {";
            for (const auto& instrumentId : instrumentIds.value())
                result += std::to_string(instrumentId.value()) + ", ";
            result += "}, ";
        }

        result += ")";

        return result;
    }

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
        auto predicate = filter::makeEmptyPredicate<Stock>();

        if (symbols.has_value())
            predicate &= HasSymbol(symbols.value());

        if (stockIds.has_value())
            predicate &= HasStockId(stockIds.value());

        if (instrumentIds.has_value())
            predicate &= HasInstrumentId(instrumentIds.value());

        return predicate;
    }

}   // namespace finance

#include "finance/instrument/instrument_predicates.hpp"

#include "filter/predicate.hpp"
#include "finance/filter/predicates.hpp"
#include "finance/instrument/option.hpp"
#include "finance/instrument/stock.hpp"

namespace finance
{

    /**
     * @brief Create a set of predicates for filtering stocks based on the
     * criteria specified in the StockFilter struct. This function generates a
     * composite predicate that can be used to filter stocks by their IDs,
     * instrument IDs, and tickers.
     *
     * @return filter::Predicate<Stock> A composite predicate for filtering
     * stocks.
     */
    filter::Predicate<Stock> StockFilter::makePredicates() const
    {
        filter::Predicate<Stock> predicate =
            filter::makeEmptyPredicate<Stock>();

        if (!stockIds.empty())
        {
            predicate &= checkId<Stock>(stockIds);
        }

        if (!instrumentIds.empty())
        {
            predicate &= checkInstrumentId<Stock>(instrumentIds);
        }

        if (!tickers.empty())
        {
            predicate &= filter::makePredicate<Stock>(
                [this](const Stock& stock)
                { return tickers.contains(stock.getTicker()); }
            );
        }

        return predicate;
    }

    filter::Predicate<Option> HasOptionName(const std::string& name)
    {
        return filter::makePredicate<Option>(
            [name](const Option& option) { return option.getName() == name; }
        );
    }

}   // namespace finance
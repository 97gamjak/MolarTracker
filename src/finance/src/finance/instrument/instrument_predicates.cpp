#include "finance/instrument/instrument_predicates.hpp"

#include "filter/predicate.hpp"
#include "finance/filter/predicates.hpp"
#include "finance/instrument/option.hpp"
#include "finance/instrument/stock.hpp"

namespace finance
{

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

    /**
     * @brief Create a Predicate to filter stocks by ticker
     *
     * @param ticker The ticker symbol to filter by
     * @return filter::Predicate<Stock>
     */
    filter::Predicate<Stock> HasTicker(const std::string& ticker)
    {
        return filter::makePredicate<Stock>(
            [ticker](const Stock& stock) { return stock.getTicker() == ticker; }
        );
    }

    filter::Predicate<Option> HasOptionName(const std::string& name)
    {
        return filter::makePredicate<Option>(
            [name](const Option& option) { return option.getName() == name; }
        );
    }

}   // namespace finance
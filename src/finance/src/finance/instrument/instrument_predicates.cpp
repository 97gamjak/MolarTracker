#include "finance/instrument/instrument_predicates.hpp"

#include "filter/predicate.hpp"
#include "finance/instrument/option.hpp"
#include "finance/instrument/stock.hpp"

namespace finance
{
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

    /**
     * @brief Create a Predicate to filter stocks by instrument ID
     *
     * @param id The instrument ID to filter by
     * @return filter::Predicate<Stock>
     */
    filter::Predicate<Stock> HasInstrumentId(InstrumentId id)
    {
        return filter::makePredicate<Stock>(
            [id](const Stock& stock) { return stock.getInstrumentId() == id; }
        );
    }

    /**
     * @brief Create a Predicate to filter stocks by instrument ID
     *
     * @param ids The instrument ID to filter by
     * @return filter::Predicate<Stock>
     */
    filter::Predicate<Stock> HasInstrumentId(const idSet<InstrumentId>& ids)
    {
        return filter::makePredicate<Stock>(
            [ids](const Stock& stock)
            { return ids.contains(stock.getInstrumentId()); }
        );
    }

    filter::Predicate<Option> HasOptionName(const std::string& name)
    {
        return filter::makePredicate<Option>(
            [name](const Option& option) { return option.getName() == name; }
        );
    }

}   // namespace finance
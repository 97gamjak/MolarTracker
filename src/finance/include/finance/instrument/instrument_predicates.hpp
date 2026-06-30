#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__INSTRUMENT_PREDICATES_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__INSTRUMENT_PREDICATES_HPP__

#include <string>

#include "config/id_types.hpp"
#include "filter/predicate.hpp"
#include "utils/container/set.hpp"

namespace finance
{
    class Stock;
    class Option;

    /**
     * @brief Struct representing a filter for stocks, this struct is used to
     * define the criteria for filtering stocks based on their identifiers,
     * instrument IDs, and tickers. It provides a method to create predicates
     * that can be used to filter stocks based on the specified criteria.
     *
     */
    struct StockFilter
    {
        /// Set of stock IDs to filter by
        IdSet<StockId> stockIds;
        /// Set of instrument IDs to filter by
        IdSet<InstrumentId> instrumentIds;
        /// Set of tickers to filter by
        Set<std::string> tickers;

        [[nodiscard]]
        filter::Predicate<Stock> makePredicates() const;
    };

    [[nodiscard]]
    filter::Predicate<Option> HasOptionName(const std::string& name);

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__INSTRUMENT__INSTRUMENT_PREDICATES_HPP__
#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__INSTRUMENT_PREDICATES_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__INSTRUMENT_PREDICATES_HPP__

#include <string>

#include "config/id_types.hpp"
#include "filter/predicate.hpp"

namespace finance
{
    class Stock;

    [[nodiscard]]
    filter::Predicate<Stock> HasTicker(const std::string& ticker);

    [[nodiscard]]
    filter::Predicate<Stock> HasInstrumentId(InstrumentId id);
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__INSTRUMENT__INSTRUMENT_PREDICATES_HPP__
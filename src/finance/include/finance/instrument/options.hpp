#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTIONS_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTIONS_HPP__

#include "common/container/id_map.hpp"
#include "option.hpp"

namespace finance
{
    /**
     * @brief Represents a collection of option instruments, which are
     * financial derivatives that give the holder the right, but not the
     * obligation, to buy or sell an underlying asset at a specified price
     * (strike price) on or before a specified date (expiration date). The
     * Options class extends an IdObjectMap of Option and provides methods to
     * access individual options by their instrument ID, allowing for efficient
     * management and retrieval of option instruments in a financial context.
     *
     */
    class Options : public IdObjectMap<Option>
    {
       public:
        using IdObjectMap<Option>::IdObjectMap;

        [[nodiscard]]
        std::optional<Option> getOption(InstrumentId id) const;
    };
}   // namespace finance

#endif   //  __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTIONS_HPP__
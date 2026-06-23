#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTIONS_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTIONS_HPP__

#include "option.hpp"
#include "utils/container/id_map.hpp"

namespace finance
{
    class Options : public IdObjectMap<Option>
    {
       public:
        using IdObjectMap<Option>::IdObjectMap;

        [[nodiscard]] std::optional<std::reference_wrapper<const Option>> getOption(
            InstrumentId id
        ) const;
    };
}   // namespace finance

#endif   //  __FINANCE__INCLUDE__FINANCE__INSTRUMENT__OPTIONS_HPP__
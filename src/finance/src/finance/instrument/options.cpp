#include "finance/instrument/options.hpp"

namespace finance
{
    std::optional<std::reference_wrapper<const Option>> Options::getOption(
        InstrumentId id
    ) const
    {
        for (const auto& [optionId, option] : getItems())
        {
            if (option.getInstrumentId() == id)
                return option;
        }
        return std::nullopt;
    }

}   // namespace finance
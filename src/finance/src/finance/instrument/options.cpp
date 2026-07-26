#include "finance/instrument/options.hpp"

namespace finance
{
    /**
     * @brief Get the option associated with the given instrument ID, this will
     * search through the collection of options and return the option that
     * matches the specified instrument ID, if found.
     *
     * @param id The instrument ID of the option to retrieve.
     * @return std::optional<Option> The option associated with the given
     * instrument ID, or std::nullopt if no matching option is found.
     */
    std::optional<Option> Options::getOption(InstrumentId id) const
    {
        for (const auto& [optionId, option] : getItems())
        {
            if (option.getInstrumentId() == id)
                return option;
        }
        return std::nullopt;
    }

}   // namespace finance
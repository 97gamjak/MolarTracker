#include <algorithm>
#include <format>

#include "log_manager.hpp"

namespace logging
{
    /**
     * @brief Convert LogLevel to formatted string
     *
     * @param level
     * @return std::string
     */
    std::string LogManager::_logLevelToString(const LogLevel& level)
    {
        // TODO(97gamjak): move this length determination logic to utils
        // https://97gamjak.atlassian.net/browse/MOLTRACK-113
        const auto maxLength = std::ranges::max_element(
                                   LogLevelMeta::names,
                                   {},
                                   [](const auto& name) { return name.size(); }
        )->size();

        constexpr auto additionalPadding = 3;
        const auto     size              = maxLength + additionalPadding;

        const auto format = "{:>" + std::to_string(size) + "}";

        const auto levelStr      = std::string{LogLevelMeta::name(level)};
        const auto cleanLevelStr = "<" + levelStr + ">:";

        return std::vformat(format, std::make_format_args(cleanLevelStr));
    }

    /**
     * @brief Set the default log level, this is used to update the default log
     * level when the corresponding setting is changed
     *
     * @param level
     */
    void LogManager::setDefaultLogLevel(const LogLevel& level)
    {
        _defaultLogLevel = level;
    }

}   // namespace logging
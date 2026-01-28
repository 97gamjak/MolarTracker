#ifndef __LOGGING__LOGGING_BASE_HPP__
#define __LOGGING__LOGGING_BASE_HPP__

#include <cstddef>
#include <mstd/enum.hpp>
#include <nlohmann/json.hpp>

#define LOG_CATEGORY(X)           \
    X(app_service_profileService) \
    X(ui_mainWindow)              \
    X(ui_logging)                 \
    X(logging_manager)

MSTD_ENUM(LogCategory, std::size_t, LOG_CATEGORY)

#define LOG_LEVEL(X) \
    X(Off, 0)        \
    X(Error)         \
    X(Warning)       \
    X(Info)          \
    X(Debug)         \
    X(Trace)

MSTD_ENUM(LogLevel, std::size_t, LOG_LEVEL)

/**
 * @brief Returns the index of the given LogLevel in LogLevelMeta::values
 *
 * @TODO: make this part of LogLevelMeta
 *
 * @param level LogLevel to find
 * @return std::optional<size_t> Index of the LogLevel, or std::nullopt if not
 * found
 */
inline std::optional<size_t> indexOfLogLevel(LogLevel level)
{
    for (size_t i = 0; i < LogLevelMeta::size; ++i)
    {
        if (LogLevelMeta::values[i] == level)
            return i;
    }

    return std::nullopt;
}

NLOHMANN_JSON_NAMESPACE_BEGIN
// TODO: migrate and standardize adl_serializer usage
// for mstd enums in mstd library itself
template <>
struct adl_serializer<LogLevel>
{
    static void to_json(json& j, const LogLevel& level)
    {
        j = LogLevelMeta::name(level);
    }

    static void from_json(const json& j, LogLevel& level)
    {
        const auto levelStr = j.get<std::string_view>();
        const auto levelOpt = LogLevelMeta::from_string(levelStr);

        if (levelOpt.has_value())
            level = *levelOpt;
        else
            throw std::invalid_argument("Invalid LogLevel string");
    }
};

template <>
struct adl_serializer<LogCategory>
{
    static void to_json(json& j, const LogCategory& category)
    {
        j = LogCategoryMeta::name(category);
    }

    static void from_json(const json& j, LogCategory& category)
    {
        const auto categoryStr = j.get<std::string_view>();
        const auto categoryOpt = LogCategoryMeta::from_string(categoryStr);

        if (categoryOpt.has_value())
            category = *categoryOpt;
        else
            throw std::invalid_argument("Invalid LogCategory string");
    }
};
NLOHMANN_JSON_NAMESPACE_END

#endif   // __LOGGING__LOGGING_BASE_HPP__
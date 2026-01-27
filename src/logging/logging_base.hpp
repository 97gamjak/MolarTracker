#ifndef __LOGGING__LOGGING_BASE_HPP__
#define __LOGGING__LOGGING_BASE_HPP__

#include <cstddef>
#include <mstd/enum.hpp>

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

#endif   // __LOGGING__LOGGING_BASE_HPP__
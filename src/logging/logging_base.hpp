#ifndef __LOGGING__LOGGING_BASE_HPP__
#define __LOGGING__LOGGING_BASE_HPP__

#include <cstddef>
#include <mstd/enum.hpp>

#define LOG_CATEGORY(X)           \
    X(app_service_profileService) \
    X(ui_mainWindow)

MSTD_ENUM(LogCategory, size_t, LOG_CATEGORY)

/**
 * @brief Log levels for logging messages
 */
enum class LogLevel
{
    Off = 0,
    Error,
    Warning,
    Info,
    Debug,
    DebugVerbose,
    Trace
};

#endif   // __LOGGING__LOGGING_BASE_HPP__
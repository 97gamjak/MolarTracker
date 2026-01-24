#ifndef __LOGGING__LOGGING_BASE_HPP__
#define __LOGGING__LOGGING_BASE_HPP__

#include <cstddef>
#include <mstd/enum.hpp>

#define LOG_CATEGORY(X)           \
    X(app_service_profileService) \
    X(ui_mainWindow)

MSTD_ENUM(LogCategory, std::size_t, LOG_CATEGORY)

#define LOG_LEVEL(X) \
    X(Off, 0)        \
    X(Error)         \
    X(Warning)       \
    X(Info)          \
    X(Debug)         \
    X(Trace)

MSTD_ENUM(LogLevel, std::size_t, LOG_LEVEL)

#endif   // __LOGGING__LOGGING_BASE_HPP__
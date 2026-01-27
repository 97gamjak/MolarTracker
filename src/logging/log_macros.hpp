#ifndef __LOGGING__LOG_MACROS_HPP__
#define __LOGGING__LOG_MACROS_HPP__

#include "log_entry_scope.hpp"
#include "log_manager.hpp"
#include "mstd/error.hpp"

// clang-format off
#define LOG_INTERNAL(level, category, message)  \
    LogManager::getInstance().log(              \
        level,                                  \
        category,                               \
        __FILE__,                               \
        __LINE__,                               \
        __func__,                               \
        message                                 \
    )
// clang-format on

// clang-format off
#ifndef __LOG_CATEGORY__
    #define LOG(level, message)                                                 \
        MSTD_COMPILE_FAIL(                                                      \
            "LOG_CATEGORY must be defined before including log_macros.hpp"      \
        );
#else
    #define LOG(level, message) LOG_INTERNAL(level, __LOG_CATEGORY__, message)
#endif
// clang-format on

#define LOG_TRACE(message)   LOG(LogLevel::Trace, message)
#define LOG_DEBUG(message)   LOG(LogLevel::Debug, message)
#define LOG_INFO(message)    LOG(LogLevel::Info, message)
#define LOG_WARNING(message) LOG(LogLevel::Warning, message)
#define LOG_ERROR(message)   LOG(LogLevel::Error, message)

// clang-format off
#define LOG_ENTRY                        \
    LogEntryScope __logEntryScope__(     \
        LogLevel::Trace,                 \
        __LOG_CATEGORY__,                \
        __func__,                        \
        __FILE__,                        \
        __LINE__                         \
    )
// clang-format on

#endif   // __LOGGING__LOG_MACROS_HPP__
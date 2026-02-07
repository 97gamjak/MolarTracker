#ifndef __LOGGING__LOG_MACROS_HPP__
#define __LOGGING__LOG_MACROS_HPP__

#include "log_entry_scope.hpp"
#include "log_manager.hpp"
#include "log_object.hpp"
#include "mstd/error.hpp"

#define LOG_OBJECT_INTERNAL(level, category, message) \
    (LogObject{level, category, message, __FILE__, __LINE__, __func__})

// clang-format off
#ifndef __LOG_CATEGORY__
    #define LOG_OBJECT(level, message)                                          \
        MSTD_COMPILE_FAIL(                                                      \
            "LOG_CATEGORY must be defined before including log_macros.hpp"      \
        );
#else
    #define LOG_OBJECT(level, message) LOG_OBJECT_INTERNAL(level, __LOG_CATEGORY__, message)
#endif
// clang-format on

#define LOG(logObject)                              \
    do                                              \
    {                                               \
        LogManager::getInstance().log((logObject)); \
    } while (0)

#define LOG_TRACE_OBJECT(message)   LOG_OBJECT(LogLevel::Trace, message)
#define LOG_DEBUG_OBJECT(message)   LOG_OBJECT(LogLevel::Debug, message)
#define LOG_INFO_OBJECT(message)    LOG_OBJECT(LogLevel::Info, message)
#define LOG_WARNING_OBJECT(message) LOG_OBJECT(LogLevel::Warning, message)
#define LOG_ERROR_OBJECT(message)   LOG_OBJECT(LogLevel::Error, message)

#define LOG_TRACE(message)   LOG(LOG_TRACE_OBJECT(message))
#define LOG_DEBUG(message)   LOG(LOG_DEBUG_OBJECT(message))
#define LOG_INFO(message)    LOG(LOG_INFO_OBJECT(message))
#define LOG_WARNING(message) LOG(LOG_WARNING_OBJECT(message))
#define LOG_ERROR(message)   LOG(LOG_ERROR_OBJECT(message))

#define LOG_ENTRY LogEntryScope __logEntryScope__(LOG_TRACE_OBJECT(""))
#define LOG_TIMED_ENTRY \
    TimedLogEntryScope __timedLogEntryScope__(LOG_TRACE_OBJECT(""))

#endif   // __LOGGING__LOG_MACROS_HPP__
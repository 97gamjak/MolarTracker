#ifndef __LOGGING__LOG_MACROS_HPP__
#define __LOGGING__LOG_MACROS_HPP__

#include <iostream>   // IWYU pragma: keep

#include "log_category.hpp"      // IWYU pragma: keep
#include "log_entry_scope.hpp"   // IWYU pragma: keep
#include "log_manager.hpp"       // IWYU pragma: keep
#include "log_object.hpp"        // IWYU pragma: keep

#define REGISTER_LOG_CATEGORY(name_literal)                       \
    namespace                                                     \
    {                                                             \
                                                                  \
        [[maybe_unused]] const logging::LogCategory _logCategory{ \
            name_literal                                          \
        };                                                        \
    }

#define LOG_OBJECT_INTERNAL(level, category, message) \
    (logging::LogObject{level, category, message, __FILE__, __LINE__, __func__})

#define LOG_OBJECT(level, message) \
    LOG_OBJECT_INTERNAL(level, _logCategory, message)

// NOLINTBEGIN(cppcoreguidelines-avoid-do-while, cppcoreguidelines-macro-usage)
#define LOG(logObject)                                       \
    do                                                       \
    {                                                        \
        logging::LogManager::getInstance().log((logObject)); \
    } while (0)
// NOLINTEND(cppcoreguidelines-avoid-do-while, cppcoreguidelines-macro-usage)

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

#define MT_DEBUG std::cerr

#endif   // __LOGGING__LOG_MACROS_HPP__
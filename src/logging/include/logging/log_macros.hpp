#ifndef __LOGGING__LOG_MACROS_HPP__
#define __LOGGING__LOG_MACROS_HPP__

#include <iostream>   // IWYU pragma: keep
#include <map>
#include <string_view>

#include "logging/log_category.hpp"      // IWYU pragma: keep
#include "logging/log_entry_scope.hpp"   // IWYU pragma: keep
#include "logging/log_manager.hpp"       // IWYU pragma: keep
#include "logging/log_object.hpp"        // IWYU pragma: keep

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

namespace logging::detail
{
    /**
     * @brief Get the Category Map object
     *
     * @return std::map<std::string_view, std::string_view>&
     */
    inline std::map<std::string_view, std::string_view>& getCategoryMap(
    ) noexcept
    {
        try
        {
            static std::map<std::string_view, std::string_view> map;
            return map;
        }
        catch (const std::exception& e)
        {
            std::abort();   // If we can't create the map, we can't continue
        }
    }

    /**
     * @brief Get the Category object
     *
     * @param file
     * @return std::string_view
     */
    inline std::string_view getCategory(const char* file) noexcept
    {
        try
        {
            auto& map = getCategoryMap();
            auto  it  = map.find(file);
            return it != map.end() ? it->second : "Unknown";
        }
        catch (...)
        {
            std::abort();   // If we can't access the map, we can't continue
        }
    }

    /**
     * @brief Add a logging category for a file
     *
     */
    struct addLoggingCategory
    {
        /**
         * @brief Construct a new add Logging Category object
         *
         * @param file
         * @param category
         */
        addLoggingCategory(const char* file, std::string_view category) noexcept
        {
            try
            {
                auto& map = getCategoryMap();
                map[file] = category;
            }
            catch (...)
            {
                std::abort();   // If we can't modify the map, we can't continue
            }
        }
    };
}   // namespace logging::detail

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y)        CONCATENATE_DETAIL(x, y)

#define REGISTER_LOG_CATEGORY(name_literal)                    \
    namespace logging::detail                                  \
    {                                                          \
        const auto CONCATENATE(__logCategory__, __COUNTER__) = \
            addLoggingCategory{(__FILE__), name_literal};      \
    }   // NOLINT

#define LOG_OBJECT_INTERNAL(level, category, message) \
    (logging::LogObject{level, category, message, __FILE__, __LINE__, __func__})

#define LOG_OBJECT(level, message)                           \
    LOG_OBJECT_INTERNAL(                                     \
        level,                                               \
        std::string(logging::detail::getCategory(__FILE__)), \
        message                                              \
    )

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

#define LOG_ENTRY logging::LogEntryScope __logEntryScope__(LOG_TRACE_OBJECT(""))
#define LOG_TIMED_ENTRY \
    logging::TimedLogEntryScope __timedLogEntryScope__(LOG_TRACE_OBJECT(""))

#define MT_DEBUG std::cerr

// NOLINTEND(cppcoreguidelines-macro-usage)

#endif   // __LOGGING__LOG_MACROS_HPP__
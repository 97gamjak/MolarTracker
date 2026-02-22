#ifndef __LOGGING__LOG_ENTRY_SCOPE_HPP__
#define __LOGGING__LOG_ENTRY_SCOPE_HPP__

#include <chrono>

#include "log_object.hpp"

/**
 * @brief RAII scope for logging entry and exit of a code block
 *
 */
class LogEntryScope
{
   protected:
    /// The LogObject associated with this scope
    LogObject _logObject;
    /// Flag indicating whether the scope is enabled
    bool _enabled;

   public:
    explicit LogEntryScope(const LogObject& logObject);

    ~LogEntryScope();
};

/**
 * @brief RAII scope for logging entry and exit of a code block with timing
 *
 */
class TimedLogEntryScope final : public LogEntryScope
{
   private:
    /// The start time of the scope
    std::chrono::steady_clock::time_point _start;

   public:
    explicit TimedLogEntryScope(const LogObject& logObject);

    ~TimedLogEntryScope();
};

#endif   // __LOGGING__LOG_ENTRY_SCOPE_HPP__
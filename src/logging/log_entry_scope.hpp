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
    LogObject _logObject;
    bool      _enabled;

   public:
    LogEntryScope(const LogObject& logObject);

    ~LogEntryScope();
};

/**
 * @brief RAII scope for logging entry and exit of a code block with timing
 *
 */
class TimedLogEntryScope final : public LogEntryScope
{
   private:
    using Clock = std::chrono::steady_clock;

    Clock::time_point _start;

   public:
    TimedLogEntryScope(const LogObject& logObject);

    ~TimedLogEntryScope();
};

#endif   // __LOGGING__LOG_ENTRY_SCOPE_HPP__
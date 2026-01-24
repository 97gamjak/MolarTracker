#ifndef __LOGGING__LOG_ENTRY_SCOPE_HPP__
#define __LOGGING__LOG_ENTRY_SCOPE_HPP__

#include <chrono>
#include <string>

#include "logging_base.hpp"

class LogEntryScope
{
   protected:
    LogLevel    _level;
    LogCategory _category;
    std::string _function;
    std::string _file;
    int         _line;
    bool        _enabled;

   public:
    LogEntryScope(
        const LogLevel&    level,
        const LogCategory& category,
        const std::string& function,
        const std::string& file,
        const int          line
    );

    ~LogEntryScope();
};

class TimedLogEntryScope final : public LogEntryScope
{
   private:
    using Clock = std::chrono::steady_clock;

    Clock::time_point _start;

   public:
    TimedLogEntryScope(
        const LogLevel&    level,
        const LogCategory& category,
        const std::string& function,
        const std::string& file,
        const int          line
    );

    ~TimedLogEntryScope();
};

#endif   // __LOGGING__LOG_ENTRY_SCOPE_HPP__
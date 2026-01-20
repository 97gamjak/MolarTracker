#ifndef __LOGGING__LOGGING_HPP__
#define __LOGGING__LOGGING_HPP__

class LogEntryScope
{
   public:
    LogEntryScope(
        LogLevel           level,
        const LogCategory& category,
        const char*        function,
        const char*        file,
        int                line
    )
        : _level{level},
          _category{category},
          _function{function},
          _file{file},
          _line{line},
          _enabled{LogManager::instance().isEnabled(category, level)}
    {
        // TODO: document why _enabled is used and not the global logger
        // instance to check if logging should be done
        if (_enabled)
        {
            LogManager::instance().log(
                _level,
                _category,
                _file,
                _line,
                _function,
                "→ enter " + std::string(_function)
            );
        }
    }

    ~LogEntryScope()
    {
        if (_enabled)
        {
            LogManager::instance().log(
                _level,
                _category,
                _file,
                _line,
                _function,
                "← exit  " + std::string(_function)
            );
        }
    }

   private:
    LogLevel           _level;
    const LogCategory& _category;
    const char*        _function;
    const char*        _file;
    int                _line;
    bool               _enabled;
};

class TimedLogEntryScope final : public LogEntryScope
{
   public:
    using Clock = std::chrono::steady_clock;

    TimedLogEntryScope(
        LogLevel           level,
        const LogCategory& category,
        const char*        function,
        const char*        file,
        int                line
    )
        : LogEntryScope(level, category, function, file, line),
          _start{Clock::now()}
    {
    }

    ~TimedLogEntryScope()
    {
        if (_enabled)
        {
            using namespace std::chrono;

            const auto timeDifference = Clock::now() - _start;
            const auto _ms = duration_cast<milliseconds>(timeDifference);
            const auto ms  = _ms.count();

            LogManager::instance().log(
                _level,
                _category,
                _file,
                _line,
                _function,
                "⧖ exit  " + std::string(_function) +
                    " (duration: " + std::to_string(ms) + " ms)"
            );
        }
    }

   private:
    Clock::time_point _start;
};

#endif   // __LOGGING__LOGGING_HPP__
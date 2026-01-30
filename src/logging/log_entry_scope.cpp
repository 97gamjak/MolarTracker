#include "log_entry_scope.hpp"

#include "log_manager.hpp"
#include "logging_base.hpp"

/**
 * @brief Construct a new Log Entry Scope:: Log Entry Scope object
 *
 * @param level
 * @param category
 * @param function
 * @param file
 * @param line
 */
LogEntryScope::LogEntryScope(
    const LogLevel&    level,
    const LogCategory& category,
    const std::string& function,
    const std::string& file,
    const int          line
)
    : _level{level},
      _category{category},
      _function{function},
      _file{file},
      _line{line},
      _enabled{LogManager::getInstance().isEnabled(category, level)}
{
    if (_enabled)
    {
        LogManager::getInstance().log(
            _level,
            _category,
            _file,
            _line,
            _function,
            "→ enter " + std::string(_function)
        );
    }
}

/**
 * @brief Destroy the Log Entry Scope:: Log Entry Scope object
 *
 */
LogEntryScope::~LogEntryScope()
{
    if (_enabled)
    {
        LogManager::getInstance().log(
            _level,
            _category,
            _file,
            _line,
            _function,
            "← exit  " + std::string(_function)
        );
    }
}

/**
 * @brief Construct a new Timed Log Entry Scope:: Timed Log Entry Scope object
 *
 * @param level
 * @param category
 * @param function
 * @param file
 * @param line
 */
TimedLogEntryScope::TimedLogEntryScope(
    const LogLevel&    level,
    const LogCategory& category,
    const std::string& function,
    const std::string& file,
    const int          line
)
    : LogEntryScope(level, category, function, file, line), _start{Clock::now()}
{
}

/**
 * @brief Destroy the Timed Log Entry Scope:: Timed Log Entry Scope object
 *
 */
TimedLogEntryScope::~TimedLogEntryScope()
{
    if (_enabled)
    {
        using namespace std::chrono;

        const auto timeDifference = Clock::now() - _start;
        const auto _ms            = duration_cast<milliseconds>(timeDifference);
        const auto ms             = _ms.count();

        LogManager::getInstance().log(
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
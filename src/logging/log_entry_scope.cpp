#include "log_entry_scope.hpp"

#include "log_manager.hpp"

/**
 * @brief Construct a new Log Entry Scope:: Log Entry Scope object
 *
 * @param logObject The log object to use for logging entry and exit of the
 * scope
 */
LogEntryScope::LogEntryScope(const LogObject& logObject)
    : _logObject{logObject},
      _enabled{LogManager::getInstance()
                   .isEnabled(logObject.category, logObject.level)}
{
    if (_enabled)
    {
        _logObject.message = "→ enter " + std::string(_logObject.function);
        LogManager::getInstance().log(_logObject);
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
        _logObject.message = "← exit  " + std::string(_logObject.function);
        LogManager::getInstance().log(_logObject);
    }
}

/**
 * @brief Construct a new Timed Log Entry Scope:: Timed Log Entry Scope object
 *
 * This class extends LogEntryScope to also log the duration of the scope in
 * milliseconds when exiting the scope. It uses a steady clock to measure the
 * time difference between the construction and destruction of the object.
 *
 * @param logObject The log object to use for logging entry and exit of the
 * scope, the exit log will also include the duration of the scope in
 * milliseconds
 */
TimedLogEntryScope::TimedLogEntryScope(const LogObject& logObject)
    : LogEntryScope(logObject), _start{std::chrono::steady_clock::now()}
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

        const auto timeDifference = steady_clock::now() - _start;
        const auto _ms            = duration_cast<milliseconds>(timeDifference);
        const auto ms             = _ms.count();

        _logObject.message = "⧖ exit  " + std::string(_logObject.function) +
                             " (duration: " + std::to_string(ms) + " ms)";

        LogManager::getInstance().log(_logObject);
    }
}
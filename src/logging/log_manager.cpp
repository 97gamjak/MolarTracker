#include "log_manager.hpp"

#include "logging_base.hpp"

LogManager& LogManager::getInstance()
{
    static LogManager instance;
    return instance;
}

LogManager::LogManager() { initCategoryMap(); }

void LogManager::initCategoryMap()
{
    for (const auto category : LogCategoryMeta::values)
        _categories[category] = LogLevel::Info;   // Default log level
}

bool LogManager::isEnabled(
    const LogCategory& category,
    const LogLevel&    level
) const
{
    return _categories.at(category) >= level;
}

void LogManager::setLogLevel(const LogCategory& category, const LogLevel& level)
{
    _categories[category] = level;
}

// void LogManager::log(
//     LogLevel           level,
//     const LogCategory& category,
//     const char*        file,
//     int                line,
//     const char*        function,
//     const std::string& message
// )
// {
//     // TODO: Implementation to log the message
// }
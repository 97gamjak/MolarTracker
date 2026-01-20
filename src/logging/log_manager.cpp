#include "log_manager.hpp"

#include <iostream>

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

void LogManager::log(
    const LogLevel&    level,
    const LogCategory& category,
    const std::string& /*file*/,
    const int          line,
    const std::string& function,
    const std::string& message
)
{
    if (!isEnabled(category, level))
        return;

    // TODO(97gamjak): implement global Timestamp helper, probably best to move
    // to mstd later on
    // https://97gamjak.atlassian.net/browse/MOLTRACK-46
    const auto timeStamp = "";

    std::string buffer;

    // TODO: implement this based on the level - not all logging needs for
    // example line and function
    buffer = timeStamp + line + function + message;

    // TODO: Replace with proper logging sink(s)
    std::cout << buffer << std::endl;
}
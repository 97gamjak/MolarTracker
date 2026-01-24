#include "log_manager.hpp"

#include <format>
#include <iostream>
#include <ranges>

#include "logging_base.hpp"
#include "utils/ring_file.hpp"
#include "utils/timestamp.hpp"

LogManager& LogManager::getInstance()
{
    static LogManager instance;
    return instance;
}

LogManager::LogManager() { _initCategoryMap(); }

/**
 * @brief Initializes the ring file logger
 *
 * @param directory Directory where the log files will be stored
 */
void LogManager::initializeRingFileLogger(
    const std::filesystem::path& configDir
)
{
    RingFile::Config config;
    _logDirectory    = configDir / "logs";
    config.directory = _logDirectory;
    config.baseName  = "molar_tracker_" + Timestamp::fileSafe();
    config.extension = ".log";

    // TODO: add here all possible logging stuff including name of the file!
    _ringFile = RingFile(config);
}

void LogManager::_initCategoryMap()
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
    const std::string& file,
    const int          line,
    const std::string& function,
    const std::string& message
)
{
    if (!isEnabled(category, level))
        return;

    std::string buffer;

    buffer += _logLevelToString(level);
    buffer += " [" + Timestamp::iso8601TimeMs() + "] ";
    buffer += message;
    if (level >= LogLevel::Debug)
    {
        buffer += " (";
        buffer += file + ":" + std::to_string(line);
        buffer += " in ";
        buffer += function;
        buffer += ")";
    }

    _ringFile.writeLine(buffer);
}

std::string LogManager::_logLevelToString(const LogLevel& level) const
{
    const auto maxLength = std::ranges::max_element(
                               LogLevelMeta::names,
                               {},
                               [](const auto& name) { return name.size(); }
    )->size();

    const auto format = "{:*>" + std::to_string(maxLength) + "}";

    const auto levelStr      = std::string{LogLevelMeta::name(level)};
    const auto cleanLevelStr = "<" + levelStr + ">:";

    return std::vformat(format, std::make_format_args(cleanLevelStr));
}
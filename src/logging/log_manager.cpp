#include "log_manager.hpp"

#include <format>
#include <iostream>
#include <ranges>

#include "logging_base.hpp"
#include "utils/ring_file.hpp"
#include "utils/timestamp.hpp"

/**
 * @brief Get the singleton instance of LogManager
 *
 * @return LogManager&
 */
LogManager& LogManager::getInstance()
{
    static LogManager instance;
    return instance;
}

/**
 * @brief Construct a new Log Manager:: Log Manager object
 *
 */
LogManager::LogManager() { _categories = getDefaultCategories(); }

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

    // TODO(97gamjak): add here all possible logging stuff including name of the
    // file!
    // https://97gamjak.atlassian.net/browse/MOLTRACK-83
    _ringFile = RingFile(config);
}

/**
 * @brief Get the default logging categories with Info level
 *
 * @return std::unordered_map<LogCategory, LogLevel>
 */
std::unordered_map<LogCategory, LogLevel> LogManager::getDefaultCategories(
) const
{
    // TODO(97gamjak): make the default levels configurable
    // https://97gamjak.atlassian.net/browse/MOLTRACK-84
    std::unordered_map<LogCategory, LogLevel> defaultCategories;
    for (const auto category : LogCategoryMeta::values)
        defaultCategories[category] = LogLevel::Info;

    return defaultCategories;
}

/**
 * @brief Get the current log file path
 *
 * @return std::filesystem::path
 */
std::filesystem::path LogManager::getCurrentLogFilePath() const
{
    return _ringFile.getCurrentLogFilePath();
}

/**
 * @brief Check if logging is enabled for the given category and level
 *
 * @param category
 * @param level
 * @return true
 * @return false
 */
bool LogManager::isEnabled(
    const LogCategory& category,
    const LogLevel&    level
) const
{
    return _categories.at(category) >= level;
}

/**
 * @brief Flush the log file
 *
 */
void LogManager::flush() { _ringFile.flush(); }

/**
 * @brief Change the log level for a given category
 *
 * @param category
 * @param level
 */
void LogManager::changeLogLevel(
    const LogCategory& category,
    const LogLevel&    level
)
{
    if (_categories[category] != level)
    {
        const auto previousLevel = _categories[category];

        _categories[category] = level;
        log(LogLevel::Info,
            LogCategory::logging_manager,
            __FILE__,
            __LINE__,
            __func__,
            std::format(
                "Log level for category '{}' changed from '{}' to '{}'",
                std::string{LogCategoryMeta::name(category)},
                std::string{LogLevelMeta::name(previousLevel)},
                std::string{LogLevelMeta::name(level)}
            ));
    }
}

/**
 * @brief Get the current logging categories and their levels
 *
 * @return std::unordered_map<LogCategory, LogLevel>
 */
std::unordered_map<LogCategory, LogLevel> LogManager::getCategories() const
{
    return _categories;
}

/**
 * @brief Log a message
 *
 * @param level
 * @param category
 * @param file
 * @param line
 * @param function
 * @param message
 */
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
    if (level >= LogLevel::Debug || level == LogLevel::Error)
    {
        buffer += " (";
        buffer += file + ":" + std::to_string(line);
        buffer += " in ";
        buffer += function;
        buffer += ")";
    }

    _ringFile.writeLine(buffer);
}

/**
 * @brief Convert LogLevel to formatted string
 *
 * @param level
 * @return std::string
 */
std::string LogManager::_logLevelToString(const LogLevel& level) const
{
    // TODO: move this length determination logic to utils
    // https://97gamjak.atlassian.net/browse/MOLTRACK-113
    const auto maxLength = std::ranges::max_element(
                               LogLevelMeta::names,
                               {},
                               [](const auto& name) { return name.size(); }
    )->size();

    constexpr auto additionalPadding = 3;
    const auto     size              = maxLength + additionalPadding;

    const auto format = "{:*>" + std::to_string(size) + "}";

    std::cout << format << std::endl;

    const auto levelStr      = std::string{LogLevelMeta::name(level)};
    const auto cleanLevelStr = "<" + levelStr + ">:";

    return std::vformat(format, std::make_format_args(cleanLevelStr));
}
#include "log_manager.hpp"

#include <format>

#include "config/logging_base.hpp"
#include "log_categories.gen.hpp"
#include "log_category.hpp"
#include "log_object.hpp"
#include "settings/logging_settings.hpp"
#include "utils/ring_file.hpp"
#include "utils/timestamp.hpp"

namespace logging
{

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

    void LogManager::initializeCategories()
    {
        _categories = LogCategories{
            CategoryRegistry::getInstance().categories,
            _defaultLogLevel
        };

        _startupCategories = _categories;
    }

    /**
     * @brief Initializes the ring file logger
     *
     * @param settings Logging settings containing the configuration for the
     * ring file logger
     * @param directory Directory where the log files will be stored
     */
    void LogManager::initializeRingFileLogger(
        const settings::LoggingSettings& settings,
        const std::filesystem::path&     directory
    )
    {
        RingFileConfig config;

        config.directory = directory / settings.getLogDirectory();
        config.baseName  = settings.getLogFilePrefix() + Timestamp::fileSafe();
        config.extension = settings.getLogFileSuffix();
        config.maxFiles  = settings.getMaxLogFiles();
        config.maxSizeMB = settings.getMaxLogFileSizeMB();

        _ringFile = RingFile(config);
    }

    /**
     * @brief Get the default logging categories with Info level
     *
     * @return LogCategories
     */
    LogCategories LogManager::getDefaultCategories() const
    {
        // make a copy of the startup categories and set all log levels to the
        // default log level
        LogCategories defaultCategories = _startupCategories;

        for (const auto& category : _startupCategories.getCategories())
        {
            defaultCategories.setLogLevel(category.getName(), _defaultLogLevel);
        }

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
        const std::string& category,
        const LogLevel&    level
    ) const
    {
        const auto& categoryOpt = _categories.getCategory(category);

        if (!categoryOpt.has_value())
            return false;

        return level >= categoryOpt->getLogLevel();
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
        const auto categoryOpt = _categories.getCategory(category.getName());

        if (!categoryOpt.has_value())
            return;

        const auto previousLevel = categoryOpt->getLogLevel();

        if (previousLevel == level)
            return;

        _categories.setLogLevel(category.getName(), level);

        const auto logObject = LogObject{
            LogLevel::Info,
            category.getName(),
            std::format(
                "Log level for category '{}' changed from '{}' to '{}'",
                std::string{_categories.getCategory(category.getName())
                                .value()
                                .getName()},
                std::string{LogLevelMeta::name(previousLevel)},
                std::string{LogLevelMeta::name(level)}
            ),
            __FILE__,
            __LINE__,
            __func__
        };
        log(logObject);
    }

    /**
     * @brief Get the current logging categories
     *
     * @return LogCategories
     */
    LogCategories LogManager::getCategories() const { return _categories; }

    /**
     * @brief Log a message
     *
     * @param logObject The log object containing the log message and
     metadata
     */
    void LogManager::log(const LogObject& logObject)
    {
        if (!isEnabled(logObject.category, logObject.level))
            return;

        std::string buffer;

        buffer += _logLevelToString(logObject.level);
        buffer += " [" + Timestamp::iso8601TimeMs() + "] ";
        buffer += logObject.message;
        if (logObject.level >= LogLevel::Debug ||
            logObject.level == LogLevel::Error)
        {
            buffer += " (";
            buffer += logObject.file + ":" + std::to_string(logObject.line);
            buffer += " in ";
            buffer += logObject.function;
            buffer += ")";
        }

        _ringFile.writeLine(buffer);

        if (logObject.level == LogLevel::Error ||
            logObject.level == LogLevel::Warning)
            flush();
    }

    LogCategory LogManager::getCategory(const std::string& name) const
    {
        const auto categoryOpt = _categories.getCategory(name);

        if (!categoryOpt.has_value())
            return LogCategory::createRootCategory(_defaultLogLevel);

        return categoryOpt.value();
    }

    /**
     * @brief Convert LogLevel to formatted string
     *
     * @param level
     * @return std::string
     */
    std::string LogManager::_logLevelToString(const LogLevel& level)
    {
        // TODO(97gamjak): move this length determination logic to utils
        // https://97gamjak.atlassian.net/browse/MOLTRACK-113
        const auto maxLength = std::ranges::max_element(
                                   LogLevelMeta::names,
                                   {},
                                   [](const auto& name) { return name.size(); }
        )->size();

        constexpr auto additionalPadding = 3;
        const auto     size              = maxLength + additionalPadding;

        const auto format = "{:>" + std::to_string(size) + "}";

        const auto levelStr      = std::string{LogLevelMeta::name(level)};
        const auto cleanLevelStr = "<" + levelStr + ">:";

        return std::vformat(format, std::make_format_args(cleanLevelStr));
    }

    /**
     * @brief Set the default log level, this is used to update the default log
     * level when the corresponding setting is changed
     *
     * @param level
     */
    void LogManager::setDefaultLogLevel(const LogLevel& level)
    {
        _defaultLogLevel = level;
    }

}   // namespace logging
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
        for (const auto& category : CategoryRegistry::getInstance().categories)
        {
            const auto& id = _addLogCategory(category, _defaultLogLevel);
            assert(id != InvalidLogCategoryId);
        }

        _defaultCategories = _categories;
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
     * @return LogCategoryMap
     */
    std::vector<LogCategory> LogManager::getDefaultCategories() const
    {
        auto defaultCategories = _startupCategories;
        for (auto& category : defaultCategories)
        {
            category.setLogLevel(LogLevel::Info);
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
        const LogCategory& category,
        const LogLevel&    level
    ) const
    {
        return _categories(category) >= level;
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
            const auto logObject  = LogObject{
                LogLevel::Info,
                LogCategory{"logging_manager"},
                std::format(
                    "Log level for category '{}' changed from '{}' to '{}'",
                    std::string{category.getName()},
                    std::string{LogLevelMeta::name(previousLevel)},
                    std::string{LogLevelMeta::name(level)}
                ),
                __FILE__,
                __LINE__,
                __func__
            };
            log(logObject);
        }
    }

    /**
     * @brief Get the current logging categories
     *
     * @return LogCategory
     */
    LogCategory LogManager::getCategories() const { return _rootCategory; }

    /**
     * @brief Log a message
     *
     * @param logObject The log object containing the log message and metadata
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

    LogCategoryId LogManager::_findLogCategory(
        const std::string& categoryName
    ) const
    {
        auto it = _categoryNameToIdMap.find(categoryName);
        if (it != _categoryNameToIdMap.end())
            return it->second;

        return InvalidLogCategoryId;
    }

    LogCategoryId LogManager::_getOrCreateLogCategory(
        LogCategoryId      parentCategoryId,
        const std::string& segment,
        const std::string& fullName,
        const LogLevel&    logLevel
    )
    {
        std::string fullNameCopy = fullName;

        _appendSegmentToFullName(fullNameCopy, segment);

        const auto existingId = _findLogCategory(fullNameCopy);

        if (existingId != InvalidLogCategoryId)
            return existingId;

        return _addLogCategory(
            parentCategoryId,
            segment,
            fullNameCopy,
            logLevel
        );
    }

    LogCategoryId LogManager::_addLogCategory(
        const std::string& fullName,
        const LogLevel&    logLevel
    )
    {
        if (fullName.empty())
            return RootLogCategoryId;

        const auto existingId = _findLogCategory(fullName);

        if (existingId != InvalidLogCategoryId)
            return existingId;

        LogCategoryId parentId = RootLogCategoryId;
        std::string   segment;
        segment.reserve(fullName.size());

        _forEachSegment(
            fullName,
            [&](const std::string& seg)
            {
                parentId =
                    _getOrCreateLogCategory(parentId, seg, fullName, logLevel);
            }
        );

        return parentId;
    }

    LogCategoryId LogManager::_addLogCategory(
        const LogCategoryId& parentCategoryId,
        const std::string&   segment,
        const std::string&   fullName,
        const LogLevel&      logLevel
    )
    {
        const auto newId = static_cast<LogCategoryId>(_categories.size());
        _categories
            .emplace_back(newId, parentCategoryId, segment, fullName, logLevel);
        _categoryNameToIdMap[fullName] = newId;
        _categories[parentCategoryId].addSubCategory(newId);
        return newId;
    }

    void LogManager::_appendSegmentToFullName(
        std::string&       parentFullName,
        const std::string& segment
    )
    {
        if (!parentFullName.empty())
            parentFullName += ".";

        parentFullName += segment;
    }

}   // namespace logging
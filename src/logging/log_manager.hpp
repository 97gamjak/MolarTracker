#ifndef __LOGGING__LOG_MANAGER_HPP__
#define __LOGGING__LOG_MANAGER_HPP__

#include <filesystem>
#include <string>
#include <unordered_map>

#include "log_category.hpp"
#include "utils/ring_file.hpp"

enum class LogLevel : std::int8_t;   // forward declaration

namespace settings
{
    class LoggingSettings;   // forward declaration
}   // namespace settings

namespace logging
{
    class LogObject;   // forward declaration

    /**
     * @brief Singleton class managing logging categories and levels
     *
     */
    class LogManager
    {
       private:
        /// Type alias for the log category map, which maps log categories to
        /// their current log levels
        using LogCategoryMap =
            std::unordered_map<LogCategory, LogLevel, LogCategory::Hash>;

        /// Mapping of log categories to their current log levels.
        LogCategoryMap _categories;

        /// The ring file logger instance used for logging to files.
        RingFile _ringFile;

        /// The directory where log files are stored.
        std::filesystem::path _logDirectory;

        /// The default log level for categories that are not explicitly set.
        LogLevel _defaultLogLevel;

       public:
        static LogManager& getInstance();

        void initializeLoggingCategories();
        void initializeRingFileLogger(
            const settings::LoggingSettings& settings,
            const std::filesystem::path&     directory
        );
        void changeLogLevel(const LogCategory& category, const LogLevel& level);
        bool isEnabled(
            const LogCategory& category,
            const LogLevel&    level
        ) const;
        void flush();

        LogCategoryMap getCategories() const;
        LogCategoryMap getDefaultCategories() const;

        std::filesystem::path getCurrentLogFilePath() const;

        void log(const LogObject& logObject);

        // TODO(97gamjak): this should be done via a command and undo stack and
        // should also be private
        // https://97gamjak.atlassian.net/browse/MOLTRACK-102
        void setDefaultLogLevel(const LogLevel& level);

       private:
        LogManager();

        static std::string _logLevelToString(const LogLevel& level);
    };

}   // namespace logging

#endif   // __LOGGING__LOG_MANAGER_HPP__
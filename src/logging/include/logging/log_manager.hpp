#ifndef __LOGGING__INCLUDE__LOGGING__LOG_MANAGER_HPP__
#define __LOGGING__INCLUDE__LOGGING__LOG_MANAGER_HPP__

#include <memory>
#include <string>
#include <string_view>

#include "config/logging_base.hpp"
#include "logging/log_categories.hpp"
#include "logging/log_category.hpp"

namespace settings
{
    class LoggingSettings;   // forward declaration
}   // namespace settings

class RingFile;   // forward declaration

namespace logging
{
    struct LogObject;   // forward declaration

    /**
     * @brief Singleton class managing logging categories and levels
     *
     */
    class LogManager
    {
       private:
        /// The collection of log categories, organized in a hierarchical
        /// structure
        LogCategories _categories;

        /// A copy of the initial log categories at startup, used for resetting
        /// to default categories
        LogCategories _startupCategories;

        /// The ring file logger instance used for logging to files.
        std::unique_ptr<RingFile> _ringFile;

        /// The directory where log files are stored.
        std::string_view _logDirectory;

        /// The default log level for categories that are not explicitly set.
        LogLevel _defaultLogLevel = LogLevel::Trace;

       public:
        static LogManager& getInstance();

        void initializeCategories();
        void initializeRingFileLogger(
            const settings::LoggingSettings& settings,
            std::string_view                 directory
        );

        void changeLogLevel(const LogCategory& category, const LogLevel& level);
        bool isEnabled(
            const std::string& categoryName,
            const LogLevel&    level
        ) const;

        void flush();

        [[nodiscard]] LogCategories getCategories() const;
        [[nodiscard]] LogCategories getDefaultCategories() const;

        std::string getCurrentLogFilePath() const;

        void log(const LogObject& logObject);

        // TODO(97gamjak): this should be done via a command and undo stack and
        // should also be private
        // https://97gamjak.atlassian.net/browse/MOLTRACK-102
        void setDefaultLogLevel(const LogLevel& level);

        LogCategory getCategory(const std::string& name) const;

       private:
        LogManager() = default;

        [[nodiscard]] static std::string _logLevelToString(
            const LogLevel& level
        );
    };

}   // namespace logging

#endif   // __LOGGING__INCLUDE__LOGGING__LOG_MANAGER_HPP__
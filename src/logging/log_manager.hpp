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
        std::vector<LogCategory>                       _categories;
        std::vector<LogCategory>                       _startupCategories;
        std::unordered_map<std::string, LogCategoryId> _categoryNameToIdMap;

        /// The ring file logger instance used for logging to files.
        RingFile _ringFile;

        /// The directory where log files are stored.
        std::filesystem::path _logDirectory;

        /// The default log level for categories that are not explicitly set.
        LogLevel _defaultLogLevel = LogLevel::Info;

       public:
        static LogManager& getInstance();

        void initializeCategories();
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

        std::vector<LogCategory> getCategories() const;
        std::vector<LogCategory> getDefaultCategories() const;

        std::filesystem::path getCurrentLogFilePath() const;

        void log(const LogObject& logObject);

        // TODO(97gamjak): this should be done via a command and undo stack and
        // should also be private
        // https://97gamjak.atlassian.net/browse/MOLTRACK-102
        void setDefaultLogLevel(const LogLevel& level);

       private:
        LogManager() = default;

        [[nodiscard]] static std::string _logLevelToString(
            const LogLevel& level
        );

        [[nodiscard]] LogCategoryId _findLogCategory(
            const std::string& categoryName
        ) const;

        [[nodiscard]] LogCategoryId _getOrCreateLogCategory(
            LogCategoryId      parentCategoryId,
            const std::string& segment,
            const std::string& fullName,
            const LogLevel&    logLevel
        );

        static void _appendSegmentToFullName(
            std::string&       parentFullName,
            const std::string& segment
        );

        [[nodiscard]] LogCategoryId _addLogCategory(
            const std::string& fullName,
            const LogLevel&    logLevel
        );

        [[nodiscard]] LogCategoryId _addLogCategory(
            const LogCategoryId& parentCategoryId,
            const std::string&   segment,
            const std::string&   fullName,
            const LogLevel&      logLevel
        );

        template <typename Fn>
        void _forEachSegment(const std::string& fullName, Fn&& fn) const
        {
            std::size_t pos = 0;
            while (pos < fullName.size())
            {
                const auto dot = fullName.find('.', pos);
                const auto end =
                    (dot == std::string::npos) ? fullName.size() : dot;

                fn(fullName.substr(pos, end - pos));

                pos = end + 1;
            }
        }
    };

}   // namespace logging

#endif   // __LOGGING__LOG_MANAGER_HPP__
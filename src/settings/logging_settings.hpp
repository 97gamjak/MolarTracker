#ifndef __SETTINGS__LOGGING_SETTINGS_HPP__
#define __SETTINGS__LOGGING_SETTINGS_HPP__

#include "config/logging_base.hpp"
#include "exceptions/base.hpp"
#include "params/params.hpp"

namespace settings
{
    /**
     * @brief Schema for logging settings
     *
     * This struct defines the keys, titles, descriptions, and default values
     * for the logging-related settings. It serves as a centralized reference
     * for all logging settings.
     */
    class LoggingSettingsSchema
    {
       public:
        // logging keys and metadata
        static constexpr const char* LOGGING_SETTINGS_KEY = "loggingSettings";
        static constexpr const char* LOGGING_SETTINGS_TITLE =
            "Logging Settings";
        static constexpr const char* LOGGING_SETTINGS_DESC =
            "Settings related to application logging.";

        // log directory keys and metadata
        static constexpr const char* LOG_DIRECTORY_KEY   = "logDirectory";
        static constexpr const char* LOG_DIRECTORY_TITLE = "Log Directory";
        static constexpr const char* LOG_DIRECTORY_DESC =
            "Directory where log files are stored. Changing this setting will "
            "require a restart of the application to take effect.";
        static constexpr const char* LOG_DIRECTORY_DEFAULT = "logs";

        // log file prefix keys and metadata
        static constexpr const char* LOG_FILE_PREFIX_KEY   = "logFilePrefix";
        static constexpr const char* LOG_FILE_PREFIX_TITLE = "Log File Prefix";
        static constexpr const char* LOG_FILE_PREFIX_DESC =
            "Prefix for log file names. The actual log files will be named "
            "using this prefix followed by a timestamp. Changing this setting "
            "will require a restart of the application to take effect.";
        static constexpr const char* LOG_FILE_PREFIX_DEFAULT = "molar_tracker_";

        // log file suffix keys and metadata
        static constexpr const char* LOG_FILE_SUFFIX_KEY   = "logFileSuffix";
        static constexpr const char* LOG_FILE_SUFFIX_TITLE = "Log File Suffix";
        static constexpr const char* LOG_FILE_SUFFIX_DESC =
            "Suffix for log file names. The actual log files will be named "
            "using this suffix after the timestamp. Changing this setting "
            "will require a restart of the application to take effect.";
        static constexpr const char* LOG_FILE_SUFFIX_DEFAULT = ".log";

        // max number of files for logging
        static constexpr const char* MAX_LOG_FILES_KEY   = "maxLogFiles";
        static constexpr const char* MAX_LOG_FILES_TITLE = "Max Log Files";
        static constexpr const char* MAX_LOG_FILES_DESC =
            "Maximum number of log files to keep. When the number of log "
            "files exceeds this limit, the oldest log files will be deleted. "
            "Changing this setting will require a restart of the application "
            "to "
            "take effect.";
        static constexpr size_t MAX_LOG_FILES_DEFAULT = 10;
        static constexpr size_t MAX_LOG_FILES_MIN     = 1;

        // max file size for logging
        static constexpr const char* MAX_LOG_FILE_SIZE_MB_KEY =
            "maxLogFileSizeMB";
        static constexpr const char* MAX_LOG_FILE_SIZE_MB_TITLE =
            "Max Log File Size (MB)";
        static constexpr const char* MAX_LOG_FILE_SIZE_MB_DESC =
            "Maximum size of a log file in megabytes. When a log file exceeds "
            "this size, a new log file will be created. Changing this setting "
            "will require a restart of the application to take effect.";
        static constexpr size_t MAX_LOG_FILE_SIZE_MB_DEFAULT = 50;
        static constexpr size_t MAX_LOG_FILE_SIZE_MB_MIN     = 1;

        // default loglevel for categories
        static constexpr const char* DEFAULT_LOG_LEVEL_KEY = "defaultLogLevel";
        static constexpr const char* DEFAULT_LOG_LEVEL_TITLE =
            "Default Log Level";
        static constexpr const char* DEFAULT_LOG_LEVEL_DESC =
            "Default log level for all categories. Changing this setting will "
            "require a restart of the application to take effect.";
        static constexpr LogLevel DEFAULT_LOG_LEVEL_DEFAULT = LogLevel::Info;
    };

    /**
     * @brief Logging-related settings management
     *
     * This class manages settings related to application logging, such as the
     * log directory and log file prefix. It inherits from ParamContainerMixin
     * to provide a consistent interface for handling parameters and their
     * serialization.
     */
    class LoggingSettings : public ParamContainerMixin<LoggingSettings>
    {
       private:
        using Schema = LoggingSettingsSchema;
        friend ParamContainerMixin<LoggingSettings>;
        ParamContainer _core;

        StringParam _logDirectory{
            Schema::LOG_DIRECTORY_KEY,
            Schema::LOG_DIRECTORY_TITLE,
            Schema::LOG_DIRECTORY_DESC
        };
        StringParam _logFilePrefix{
            Schema::LOG_FILE_PREFIX_KEY,
            Schema::LOG_FILE_PREFIX_TITLE,
            Schema::LOG_FILE_PREFIX_DESC
        };
        StringParam _logFileSuffix{
            Schema::LOG_FILE_SUFFIX_KEY,
            Schema::LOG_FILE_SUFFIX_TITLE,
            Schema::LOG_FILE_SUFFIX_DESC
        };
        NumericParam<size_t> _maxLogFiles{
            Schema::MAX_LOG_FILES_KEY,
            Schema::MAX_LOG_FILES_TITLE,
            Schema::MAX_LOG_FILES_DESC
        };
        NumericParam<size_t> _maxLogFileSizeMB{
            Schema::MAX_LOG_FILE_SIZE_MB_KEY,
            Schema::MAX_LOG_FILE_SIZE_MB_TITLE,
            Schema::MAX_LOG_FILE_SIZE_MB_DESC
        };
        EnumParam<LogLevel> _defaultLogLevel{
            Schema::DEFAULT_LOG_LEVEL_KEY,
            Schema::DEFAULT_LOG_LEVEL_TITLE,
            Schema::DEFAULT_LOG_LEVEL_DESC
        };

       public:
        LoggingSettings();

        [[nodiscard]] std::string getLogDirectory() const;
        [[nodiscard]] std::string getLogFilePrefix() const;
        [[nodiscard]] std::string getLogFileSuffix() const;
        [[nodiscard]] size_t      getMaxLogFiles() const;
        [[nodiscard]] size_t      getMaxLogFileSizeMB() const;

        [[nodiscard]] nlohmann::json toJson() const;

        static void fromJson(
            const nlohmann::json& j,
            LoggingSettings&      settings
        );

       private:
        [[nodiscard]] auto _getParams() const&;
        [[nodiscard]] auto _getParams() &;
    };

    /**
     * @brief LoggingSettings related exceptions
     *
     * This class defines exceptions related to LoggingSettings, such as when a
     * required setting value is not set. It inherits from MolarTrackerException
     * to provide a consistent exception handling mechanism across the
     * application.
     */
    class LoggingSettingsException : public MolarTrackerException
    {
       public:
        explicit LoggingSettingsException(std::string message);

        const char* what() const noexcept override;
    };

}   // namespace settings

#endif   // __SETTINGS__LOGGING_SETTINGS_HPP__
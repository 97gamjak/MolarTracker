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
        /********************
         * Logging Settings *
         ********************/

        /// logging settings key
        static constexpr const char* LOGGING_SETTINGS_KEY = "loggingSettings";
        /// logging settings title
        static constexpr const char* LOGGING_SETTINGS_TITLE =
            "Logging Settings";
        /// logging settings description
        static constexpr const char* LOGGING_SETTINGS_DESC =
            "Settings related to application logging.";

        /*****************
         * Log Directory *
         *****************/

        /// log directory key
        static constexpr const char* LOG_DIRECTORY_KEY = "logDirectory";
        /// log directory title
        static constexpr const char* LOG_DIRECTORY_TITLE = "Log Directory";
        /// log directory description
        static constexpr const char* LOG_DIRECTORY_DESC =
            "Directory where log files are stored. Changing this setting will "
            "require a restart of the application to take effect.";
        /// log directory default value
        static constexpr const char* LOG_DIRECTORY_DEFAULT = "logs";

        /*******************
         * Log File Prefix *
         *******************/

        /// log file prefix key
        static constexpr const char* LOG_FILE_PREFIX_KEY = "logFilePrefix";
        /// log file prefix title
        static constexpr const char* LOG_FILE_PREFIX_TITLE = "Log File Prefix";
        /// log file prefix description
        static constexpr const char* LOG_FILE_PREFIX_DESC =
            "Prefix for log file names. The actual log files will be named "
            "using this prefix followed by a timestamp. Changing this setting "
            "will require a restart of the application to take effect.";
        /// log file prefix default value
        static constexpr const char* LOG_FILE_PREFIX_DEFAULT = "molar_tracker_";

        /*******************
         * Log File Suffix *
         *******************/

        /// log file suffix key
        static constexpr const char* LOG_FILE_SUFFIX_KEY = "logFileSuffix";
        /// log file suffix title
        static constexpr const char* LOG_FILE_SUFFIX_TITLE = "Log File Suffix";
        /// log file suffix description
        static constexpr const char* LOG_FILE_SUFFIX_DESC =
            "Suffix for log file names. The actual log files will be named "
            "using this suffix after the timestamp. Changing this setting "
            "will require a restart of the application to take effect.";
        /// log file suffix default value
        static constexpr const char* LOG_FILE_SUFFIX_DEFAULT = ".log";

        /*****************
         * Max Log Files *
         *****************/

        /// max number of files for logging key
        static constexpr const char* MAX_LOG_FILES_KEY = "maxLogFiles";
        /// max number of files for logging title
        static constexpr const char* MAX_LOG_FILES_TITLE = "Max Log Files";
        /// max number of files for logging description
        static constexpr const char* MAX_LOG_FILES_DESC =
            "Maximum number of log files to keep. When the number of log "
            "files exceeds this limit, the oldest log files will be deleted. "
            "Changing this setting will require a restart of the application "
            "to take effect.";
        /// max number of files for logging default value
        static constexpr size_t MAX_LOG_FILES_DEFAULT = 10;
        /// max number of files for logging minimum value
        static constexpr size_t MAX_LOG_FILES_MIN = 1;

        /*********************
         * Max Log File Size *
         *********************/

        /// max log file size in megabytes key
        static constexpr const char* MAX_LOG_FILE_SIZE_MB_KEY =
            "maxLogFileSizeMB";
        /// max log file size in megabytes title
        static constexpr const char* MAX_LOG_FILE_SIZE_MB_TITLE =
            "Max Log File Size (MB)";
        /// max log file size in megabytes description
        static constexpr const char* MAX_LOG_FILE_SIZE_MB_DESC =
            "Maximum size of a log file in megabytes. When a log file exceeds "
            "this size, a new log file will be created. Changing this setting "
            "will require a restart of the application to take effect.";
        /// max log file size in megabytes default value
        static constexpr size_t MAX_LOG_FILE_SIZE_MB_DEFAULT = 50;
        /// max log file size in megabytes minimum value
        static constexpr size_t MAX_LOG_FILE_SIZE_MB_MIN = 1;

        /********************
         * Default Log Level *
         ********************/

        /// default log level key
        static constexpr const char* DEFAULT_LOG_LEVEL_KEY = "defaultLogLevel";
        /// default log level title
        static constexpr const char* DEFAULT_LOG_LEVEL_TITLE =
            "Default Log Level";
        /// default log level description
        static constexpr const char* DEFAULT_LOG_LEVEL_DESC =
            "Default log level for all categories. This setting determines the "
            "minimum severity of log messages that will be recorded.";
        /// default log level default value
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
        /// type alias for LoggingSettingsSchema
        using Schema = LoggingSettingsSchema;
        /// friend declaration to allow ParamContainerMixin to access private
        /// members of LoggingSettings, this is necessary because
        /// ParamContainerMixin needs to access the _core member of
        /// LoggingSettings to implement the functionality for logging settings
        friend ParamContainerMixin<LoggingSettings>;

        /// The core container for the logging settings parameters
        ParamContainer _core;

        /// The directory where log files are stored
        StringParam _logDirectory{
            Schema::LOG_DIRECTORY_KEY,
            Schema::LOG_DIRECTORY_TITLE,
            Schema::LOG_DIRECTORY_DESC
        };

        /// The prefix for log file names
        StringParam _logFilePrefix{
            Schema::LOG_FILE_PREFIX_KEY,
            Schema::LOG_FILE_PREFIX_TITLE,
            Schema::LOG_FILE_PREFIX_DESC
        };

        /// The suffix for log file names
        StringParam _logFileSuffix{
            Schema::LOG_FILE_SUFFIX_KEY,
            Schema::LOG_FILE_SUFFIX_TITLE,
            Schema::LOG_FILE_SUFFIX_DESC
        };

        /// The maximum number of log files to keep
        NumericParam<size_t> _maxLogFiles{
            Schema::MAX_LOG_FILES_KEY,
            Schema::MAX_LOG_FILES_TITLE,
            Schema::MAX_LOG_FILES_DESC
        };

        /// The maximum size of a log file in megabytes
        NumericParam<size_t> _maxLogFileSizeMB{
            Schema::MAX_LOG_FILE_SIZE_MB_KEY,
            Schema::MAX_LOG_FILE_SIZE_MB_TITLE,
            Schema::MAX_LOG_FILE_SIZE_MB_DESC
        };

        /// The default log level for all categories
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

        [[nodiscard]] EnumParam<LogLevel>&       getDefaultLogLevelParam();
        [[nodiscard]] const EnumParam<LogLevel>& getDefaultLogLevelParam(
        ) const;

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
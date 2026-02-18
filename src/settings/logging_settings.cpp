#include "logging_settings.hpp"

namespace settings
{
    /**
     * @brief Construct a new LoggingSettings::LoggingSettings object
     *
     */
    LoggingSettings::LoggingSettings()
        : _core(
              Schema::LOGGING_SETTINGS_KEY,
              Schema::LOGGING_SETTINGS_TITLE,
              Schema::LOGGING_SETTINGS_DESC
          )
    {
        _logDirectory.setDefault(Schema::LOG_DIRECTORY_DEFAULT);
        _logFilePrefix.setDefault(Schema::LOG_FILE_PREFIX_DEFAULT);
        _logFileSuffix.setDefault(Schema::LOG_FILE_SUFFIX_DEFAULT);

        _maxLogFiles.setDefault(Schema::MAX_LOG_FILES_DEFAULT);
        _maxLogFiles.setMinValue(Schema::MAX_LOG_FILES_MIN);

        _maxLogFileSizeMB.setDefault(Schema::MAX_LOG_FILE_SIZE_MB_DEFAULT);
        _maxLogFileSizeMB.setMinValue(Schema::MAX_LOG_FILE_SIZE_MB_MIN);

        _defaultLogLevel.setDefault(Schema::DEFAULT_LOG_LEVEL_DEFAULT);

        _logDirectory.setRebootRequired(true);
        _logFilePrefix.setRebootRequired(true);
        _logFileSuffix.setRebootRequired(true);
        _maxLogFiles.setRebootRequired(true);
        _maxLogFileSizeMB.setRebootRequired(true);
    }

    /**
     * @brief Get the parameters of LoggingSettings as a tuple (const version)
     *
     * @return auto
     */
    auto LoggingSettings::_getParams() const&
    {
        return std::tie(
            _logDirectory,
            _logFilePrefix,
            _logFileSuffix,
            _maxLogFiles,
            _maxLogFileSizeMB,
            _defaultLogLevel
        );
    }

    /**
     * @brief Get the parameters of LoggingSettings as a tuple (non-const
     * version)
     *
     * @return auto
     */
    auto LoggingSettings::_getParams() &
    {
        return std::tie(
            _logDirectory,
            _logFilePrefix,
            _logFileSuffix,
            _maxLogFiles,
            _maxLogFileSizeMB,
            _defaultLogLevel
        );
    }

    /**
     * @brief Get the log directory setting
     *
     * @return std::string
     */
    std::string LoggingSettings::getLogDirectory() const
    {
        return _logDirectory.get();
    }

    /**
     * @brief Get the log file suffix setting
     *
     * @return std::string
     */
    std::string LoggingSettings::getLogFileSuffix() const
    {
        return _logFileSuffix.get();
    }

    /**
     * @brief Get the log file prefix setting
     *
     * @return std::string
     */
    std::string LoggingSettings::getLogFilePrefix() const
    {
        return _logFilePrefix.get();
    }

    /**
     * @brief Get the max log files setting
     *
     * @return size_t
     */
    size_t LoggingSettings::getMaxLogFiles() const
    {
        return _maxLogFiles.get();
    }

    /**
     * @brief Get the max log file size in MB setting
     *
     * @return size_t
     */
    size_t LoggingSettings::getMaxLogFileSizeMB() const
    {
        return _maxLogFileSizeMB.get();
    }

    /**
     * @brief Get the default log level parameter, this returns the EnumParam
     * object itself to allow for setting the value and other properties of the
     * parameter, such as the default value and whether it requires a reboot.
     *
     * @return EnumParam<LogLevel>&
     */
    [[nodiscard]] EnumParam<LogLevel>& LoggingSettings::getDefaultLogLevelParam(
    )
    {
        return _defaultLogLevel;
    }

    /**
     * @brief Get the default log level parameter (const version)
     *
     * @return const EnumParam<LogLevel>&
     */
    [[nodiscard]] const EnumParam<LogLevel>& LoggingSettings::
        getDefaultLogLevelParam() const
    {
        return _defaultLogLevel;
    }

    /**
     * @brief Get the JSON representation of the LoggingSettings
     *
     * @return nlohmann::json
     */
    nlohmann::json LoggingSettings::toJson() const
    {
        return paramsToJson(_getParams());
    }

    /**
     * @brief Deserialize LoggingSettings from JSON
     *
     * @param jsonData
     * @param settings
     */
    void LoggingSettings::fromJson(
        const nlohmann::json& jsonData,
        LoggingSettings&      settings
    )
    {
        paramsFromJson(settings._getParams(), jsonData);
    }

    /**
     * @brief Construct a new LoggingSettingsException::LoggingSettingsException
     * object
     *
     * @param message The exception message
     */
    LoggingSettingsException::LoggingSettingsException(std::string message)
        : MolarTrackerException(std::move(message))
    {
    }

    /**
     * @brief Get the exception message
     *
     * @return const char* The exception message
     */
    const char* LoggingSettingsException::what() const noexcept
    {
        static const std::string prefix = "LoggingSettingsException: ";
        static const std::string fullMessage =
            prefix + MolarTrackerException::what();

        return fullMessage.c_str();
    }

}   // namespace settings

#include "log_viewer_settings.hpp"

#include "params/params.hpp"

namespace settings
{
    /**
     * @brief Construct a new LogViewerSettings::LogViewerSettings object
     *
     */
    LogViewerSettings::LogViewerSettings()
        : _core(
              Schema::LOG_VIEWER_SETTINGS_KEY,
              Schema::LOG_VIEWER_SETTINGS_TITLE,
              Schema::LOG_VIEWER_SETTINGS_DESC
          )
    {
        _reloadIntervalSec.setDefault(Schema::RELOAD_INTERVAL_SEC_DEFAULT);
        _reloadIntervalSec.setMinValue(Schema::RELOAD_INTERVAL_SEC_MIN);
        _reloadIntervalSec.setPrecision(Schema::RELOAD_INTERVAL_SEC_PRECISION);

        _autoReload.setDefault(Schema::AUTO_RELOAD_DEFAULT);

        _lineWrap.setDefault(Schema::LINE_WRAP_DEFAULT);
    }

    /**
     * @brief Get the parameters of LogViewerSettings as a tuple (const version)
     *
     * @return auto
     */
    auto LogViewerSettings::_getParams() const&
    {
        return std::tie(_reloadIntervalSec, _autoReload, _lineWrap);
    }

    /**
     * @brief Get the parameters of LogViewerSettings as a tuple (non-const
     * version)
     *
     * @return auto
     */
    auto LogViewerSettings::_getParams() &
    {
        return std::tie(_reloadIntervalSec, _autoReload, _lineWrap);
    }

    /**
     * @brief Serialize LogViewerSettings to JSON
     *
     * @return nlohmann::json
     */
    nlohmann::json LogViewerSettings::toJson() const
    {
        return paramsToJson(_getParams());
    }

    /**
     * @brief Deserialize LogViewerSettings from JSON
     *
     * @param jsonData
     * @param settings
     */
    void LogViewerSettings::fromJson(
        const nlohmann::json& jsonData,
        LogViewerSettings&    settings
    )
    {
        paramsFromJson(settings._getParams(), jsonData);
    }

    /**
     * @brief Get the reload interval in seconds
     *
     * @return double
     */
    double LogViewerSettings::getReloadIntervalSec() const
    {
        return _reloadIntervalSec.get();
    }

    /**
     * @brief Get the auto-reload setting
     *
     * @return bool
     */
    bool LogViewerSettings::isAutoReloadEnabled() const
    {
        return _autoReload.get();
    }

    /**
     * @brief Get the line wrap setting
     *
     * @return bool
     */
    bool LogViewerSettings::isLineWrapEnabled() const
    {
        return _lineWrap.get();
    }

    /**
     * @brief Construct a new
     * LogViewerSettingsException::LogViewerSettingsException object
     *
     * @param message The exception message
     */
    LogViewerSettingsException::LogViewerSettingsException(std::string message)
        : MolarTrackerException(std::move(message))
    {
    }

    /**
     * @brief Get the exception message
     *
     * @return const char* The exception message
     */
    const char* LogViewerSettingsException::what() const noexcept
    {
        static const std::string prefix = "LogViewerSettingsException: ";
        static std::string       fullMessage;
        fullMessage = prefix + MolarTrackerException::what();
        return fullMessage.c_str();
    }

}   // namespace settings
#include "log_viewer_settings.hpp"

namespace settings
{
    /**
     * @brief Serialize LogViewerSettings to JSON
     *
     * @return nlohmann::json
     */
    nlohmann::json LogViewerSettings::toJson() const
    {
        nlohmann::json jsonData;
        jsonData[_reloadIntervalSecKey] = _reloadIntervalSec;
        jsonData[_autoReloadKey]        = _autoReload;
        return jsonData;
    }

    /**
     * @brief Deserialize LogViewerSettings from JSON
     *
     * @param j
     * @return settings::LogViewerSettings
     */
    LogViewerSettings LogViewerSettings::fromJson(const nlohmann::json& j)
    {
        LogViewerSettings settings;

        _fromJsonReloadIntervalSec(j, settings);
        _fromJsonAutoReload(j, settings);

        return settings;
    }

    /**
     * @brief Get the reload interval in seconds
     *
     * @return double
     */
    double LogViewerSettings::getReloadIntervalSec() const
    {
        return _reloadIntervalSec;
    }

    /**
     * @brief Check if auto reload is enabled
     *
     * @return bool
     */
    bool LogViewerSettings::isAutoReloadEnabled() const { return _autoReload; }

    /**
     * @brief Deserialize the reload interval from JSON
     *
     * @param jsonData
     * @param settings
     */
    void LogViewerSettings::_fromJsonReloadIntervalSec(
        const nlohmann::json& jsonData,
        LogViewerSettings&    settings
    )
    {
        const auto key          = _reloadIntervalSecKey;
        const auto defaultValue = _DEFAULT_RELOAD_INTERVAL_SEC;

        settings._reloadIntervalSec = jsonData.value(key, defaultValue);
    }

    /**
     * @brief Deserialize the auto reload setting from JSON
     *
     * @param jsonData
     * @param settings
     */
    void LogViewerSettings::_fromJsonAutoReload(
        const nlohmann::json& jsonData,
        LogViewerSettings&    settings
    )
    {
        const auto key          = _autoReloadKey;
        const auto defaultValue = _DEFAULT_AUTO_RELOAD;

        settings._autoReload = jsonData.value(key, defaultValue);
    }
}   // namespace settings
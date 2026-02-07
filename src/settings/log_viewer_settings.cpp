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

        const auto key          = _reloadIntervalSecKey;
        const auto defaultValue = __DEFAULT_RELOAD_INTERVAL_SEC;

        settings._reloadIntervalSec = j.value(key, defaultValue);

        return settings;
    }
}   // namespace settings
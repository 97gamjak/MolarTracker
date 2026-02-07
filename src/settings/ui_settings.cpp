#include "ui_settings.hpp"

namespace settings
{
    /**
     * @brief Serialize UISettings to JSON
     *
     * @param j
     * @param settings
     */
    nlohmann::json UISettings::toJson() const
    {
        nlohmann::json jsonData;

        jsonData[_logViewerSettingsKey] = _logViewerSettings;

        return jsonData;
    }

    /**
     * @brief Deserialize UISettings from JSON
     *
     * @param j
     * @return settings::UISettings
     */
    UISettings UISettings::fromJson(const nlohmann::json& j)
    {
        UISettings settings;

        const auto key          = _logViewerSettingsKey;
        const auto defaultValue = LogViewerSettings{};

        settings._logViewerSettings = j.value(key, defaultValue);

        return settings;
    }

    /**
     * @brief Get the LogViewerSettings object
     *
     * @return LogViewerSettings&
     */
    LogViewerSettings& UISettings::getLogViewerSettings()
    {
        return _logViewerSettings;
    }

    /**
     * @brief Get the LogViewerSettings object (const version)
     *
     * @return const LogViewerSettings&
     */
    const LogViewerSettings& UISettings::getLogViewerSettings() const
    {
        return _logViewerSettings;
    }

}   // namespace settings
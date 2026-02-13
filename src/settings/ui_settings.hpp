#ifndef __SETTINGS__UI_SETTINGS_HPP__
#define __SETTINGS__UI_SETTINGS_HPP__

#include "log_viewer_settings.hpp"
#include "nlohmann/json.hpp"
#include "param_container.hpp"

namespace settings
{

    /**
     * @brief Schema for UI settings
     *
     * This struct defines the keys, titles, and descriptions for the UI
     * settings. It serves as a centralized reference for all UI related
     * settings.
     */
    class UISettingsSchema
    {
       public:
        static constexpr const char* UI_SETTINGS_KEY   = "uiSettings";
        static constexpr const char* UI_SETTINGS_TITLE = "UI Settings";
        static constexpr const char* UI_SETTINGS_DESC =
            "Settings related to the user interface.";
    };

    /**
     * @brief UI-related settings management
     */
    class UISettings : public ParamContainer
    {
       private:
        using Schema = UISettingsSchema;

        LogViewerSettings _logViewerSettings;

       public:
        UISettings();

        nlohmann::json    toJson() const;
        static UISettings fromJson(const nlohmann::json& j);

       private:
        UISettings(std::string key, std::string title, std::string description);

        [[nodiscard]] auto _getParams() const&;
        [[nodiscard]] auto _getParams() &;
    };

}   // namespace settings

NLOHMANN_JSON_NAMESPACE_BEGIN
/**
 * @brief Serializer for utils::SemVer
 *
 */
template <>
struct adl_serializer<settings::UISettings>
{
    /**
     * @brief Serialize UISettings to JSON
     *
     * @param j
     * @param settings
     */
    static void to_json(nlohmann::json& j, const settings::UISettings& settings)
    {
        j = settings.toJson();
    }

    /**
     * @brief Deserialize UISettings from JSON
     *
     * @param j
     * @return settings::UISettings
     */
    static settings::UISettings from_json(const nlohmann::json& j)
    {
        return settings::UISettings::fromJson(j);
    }
};
NLOHMANN_JSON_NAMESPACE_END

#endif   // __SETTINGS__UI_SETTINGS_HPP__
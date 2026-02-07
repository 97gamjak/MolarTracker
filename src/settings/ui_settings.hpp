#ifndef __SETTINGS__UI_SETTINGS_HPP__
#define __SETTINGS__UI_SETTINGS_HPP__

#include "log_viewer_settings.hpp"
#include "nlohmann/json.hpp"

namespace settings
{

    /**
     * @brief UI-related settings management
     */
    class UISettings
    {
       private:
        // clang-format off
        static constexpr const char* _logViewerSettingsKey = "logViewerSettings";
        // clang-format on

        LogViewerSettings _logViewerSettings;

       public:
        UISettings() = default;

        nlohmann::json    toJson() const;
        static UISettings fromJson(const nlohmann::json& j);
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
#ifndef __LOG_VIEWER_SETTINGS_HPP__
#define __LOG_VIEWER_SETTINGS_HPP__

#include "nlohmann/json.hpp"

namespace settings
{

    /**
     * @brief Log viewer related settings management
     */
    class LogViewerSettings
    {
       private:
        // clang-format off
        static constexpr const char* _reloadIntervalSecKey         = "reloadIntervalSec";
        static constexpr double      _DEFAULT_RELOAD_INTERVAL_SEC  = 1.0;
        static constexpr const char* _autoReloadKey                = "autoReload";
        static constexpr bool        _DEFAULT_AUTO_RELOAD          = false;
        // clang-format on

        double _reloadIntervalSec = _DEFAULT_RELOAD_INTERVAL_SEC;
        bool   _autoReload        = _DEFAULT_AUTO_RELOAD;

       public:
        LogViewerSettings() = default;

        nlohmann::json           toJson() const;
        static LogViewerSettings fromJson(const nlohmann::json& j);

        [[nodiscard]] double getReloadIntervalSec() const;
        [[nodiscard]] bool   isAutoReloadEnabled() const;

       private:
        static void _fromJsonReloadIntervalSec(
            const nlohmann::json& jsonData,
            LogViewerSettings&    settings
        );
        static void _fromJsonAutoReload(
            const nlohmann::json& jsonData,
            LogViewerSettings&    settings
        );
    };

}   // namespace settings

NLOHMANN_JSON_NAMESPACE_BEGIN
/**
 * @brief Serializer for settings::LogViewerSettings
 *
 */
template <>
struct adl_serializer<settings::LogViewerSettings>
{
    /**
     * @brief Serialize LogViewerSettings to JSON
     *
     * @param j
     * @param settings
     */
    static void to_json(
        nlohmann::json&                    j,
        const settings::LogViewerSettings& settings
    )
    {
        j = settings.toJson();
    }

    /**
     * @brief Deserialize LogViewerSettings from JSON
     *
     * @param j
     * @return settings::LogViewerSettings
     */
    static settings::LogViewerSettings from_json(const nlohmann::json& j)
    {
        return settings::LogViewerSettings::fromJson(j);
    }
};
NLOHMANN_JSON_NAMESPACE_END

#endif   // __LOG_VIEWER_SETTINGS_HPP__
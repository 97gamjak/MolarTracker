#ifndef __LOG_VIEWER_SETTINGS_HPP__
#define __LOG_VIEWER_SETTINGS_HPP__

#include "nlohmann/json.hpp"
#include "numeric_param.hpp"

namespace settings
{
    struct LogViewerSettingsSchema
    {
        static constexpr const char* RELOAD_INTERVAL_SEC_KEY =
            "reloadIntervalSec";
        static constexpr const char* RELOAD_INTERVAL_SEC_TITLE =
            "Reload Interval (seconds)";
        static constexpr const char* RELOAD_INTERVAL_SEC_DESC =
            "Interval in seconds for reloading logs. A smaller value means "
            "more frequent updates but may increase CPU usage.";
        static constexpr double      RELOAD_INTERVAL_SEC_DEFAULT   = 1.0;
        static constexpr double      RELOAD_INTERVAL_SEC_MIN       = 0.001;
        static constexpr std::size_t RELOAD_INTERVAL_SEC_PRECISION = 3;
    };

    /**
     * @brief Log viewer related settings management
     */
    class LogViewerSettings
    {
       private:
        using Schema = LogViewerSettingsSchema;
        NumericParam<double> _reloadIntervalSec{
            Schema::RELOAD_INTERVAL_SEC_KEY,
            Schema::RELOAD_INTERVAL_SEC_TITLE
        };

       public:
        LogViewerSettings();

        nlohmann::json           toJson() const;
        static LogViewerSettings fromJson(const nlohmann::json& j);

       private:
        const auto& getParams() const;
        auto&       getParams();
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
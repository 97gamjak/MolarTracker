#ifndef __LOG_VIEWER_SETTINGS_HPP__
#define __LOG_VIEWER_SETTINGS_HPP__

#include "nlohmann/json.hpp"
#include "numeric_param.hpp"
#include "param_container.hpp"
#include "param_container_mixin.hpp"

namespace settings
{
    /**
     * @brief Schema for log viewer settings
     *
     * This struct defines the keys, titles, descriptions, default values, and
     * validation constraints for the log viewer settings parameters. It serves
     * as a centralized reference for all log viewer related settings.
     */
    struct LogViewerSettingsSchema
    {
        // general schema keys and metadata
        static constexpr const char* LOG_VIEWER_SETTINGS_KEY =
            "logViewerSettings";
        static constexpr const char* LOG_VIEWER_SETTINGS_TITLE =
            "Log Viewer Settings";
        static constexpr const char* LOG_VIEWER_SETTINGS_DESC =
            "Settings related to the log viewer component.";

        // reload interval keys and metadata
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
    class LogViewerSettings : public ParamContainerMixin<LogViewerSettings>
    {
       private:
        ParamContainer _core;
        using Schema = LogViewerSettingsSchema;

        NumericParam<double> _reloadIntervalSec{
            Schema::RELOAD_INTERVAL_SEC_KEY,
            Schema::RELOAD_INTERVAL_SEC_TITLE
        };

       public:
        LogViewerSettings();

        [[nodiscard]] ParamContainer&       core();
        [[nodiscard]] const ParamContainer& core() const;

        [[nodiscard]] nlohmann::json           toJson() const;
        [[nodiscard]] static LogViewerSettings fromJson(
            const nlohmann::json& j
        );

       private:
        [[nodiscard]] auto _getParams() const&;
        [[nodiscard]] auto _getParams() &;
    };

}   // namespace settings

#endif   // __LOG_VIEWER_SETTINGS_HPP__
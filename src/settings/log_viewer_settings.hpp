#ifndef __LOG_VIEWER_SETTINGS_HPP__
#define __LOG_VIEWER_SETTINGS_HPP__

#include "params/params.hpp"

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

        NumericParam<double> _reloadIntervalSec
        {
            Schema::RELOAD_INTERVAL_SEC_KEY,
                static constexpr const char* _autoReloadKey = "autoReload";
            static constexpr bool            _DEFAULT_AUTO_RELOAD = false;
            Schema::RELOAD_INTERVAL_SEC_TITLE
        }
        bool _autoReload = _DEFAULT_AUTO_RELOAD;

       public:
        LogViewerSettings();

        [[nodiscard]] ParamContainer&       core();
        [[nodiscard]] const ParamContainer& core() const;

        [[nodiscard]] nlohmann::json toJson() const;

        static void fromJson(
            const nlohmann::json& j,
            LogViewerSettings&    settings
        );

       private:
        [[nodiscard]] auto _getParams() const&;
        [[nodiscard]] auto _getParams() &;

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

#endif   // __LOG_VIEWER_SETTINGS_HPP__
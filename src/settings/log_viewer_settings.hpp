#ifndef __LOG_VIEWER_SETTINGS_HPP__
#define __LOG_VIEWER_SETTINGS_HPP__

#include "exceptions/base.hpp"
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
        /***********************
         * Log Viewer Settings *
         ***********************/

        /// log viewer settings key
        static constexpr const char* LOG_VIEWER_SETTINGS_KEY =
            "logViewerSettings";
        /// log viewer settings title
        static constexpr const char* LOG_VIEWER_SETTINGS_TITLE =
            "Log Viewer Settings";
        /// log viewer settings description
        static constexpr const char* LOG_VIEWER_SETTINGS_DESC =
            "Settings related to the log viewer component.";

        /*******************
         * Reload Interval *
         *******************/

        /// reload interval key
        static constexpr const char* RELOAD_INTERVAL_SEC_KEY =
            "reloadIntervalSec";
        /// reload interval title
        static constexpr const char* RELOAD_INTERVAL_SEC_TITLE =
            "Reload Interval (seconds)";
        /// reload interval description
        static constexpr const char* RELOAD_INTERVAL_SEC_DESC =
            "Interval in seconds for reloading logs. A smaller value means "
            "more frequent updates but may increase CPU usage.";
        /// reload interval default value
        static constexpr double RELOAD_INTERVAL_SEC_DEFAULT = 1.0;
        /// reload interval minimum value
        static constexpr double RELOAD_INTERVAL_SEC_MIN = 0.001;
        /// reload interval precision (number of decimal places)
        static constexpr std::size_t RELOAD_INTERVAL_SEC_PRECISION = 3;

        /***********************
         * Automatic Reloading *
         ***********************/

        /// auto reload key
        static constexpr const char* AUTO_RELOAD_KEY = "autoReload";
        /// auto reload title
        static constexpr const char* AUTO_RELOAD_TITLE =
            "Automatic Reloading Log File";
        /// auto reload description
        static constexpr const char* AUTO_RELOAD_DESC =
            "Enable or disable automatic reloading of the log file. When "
            "enabled, the log viewer will automatically reload the log file at "
            "the specified interval. When disabled, the log viewer will only "
            "reload the log file when the user manually triggers a reload.";
        /// auto reload default value
        static constexpr bool AUTO_RELOAD_DEFAULT = false;

        /*****************
         * Line Wrapping *
         *****************/

        /// line wrap key
        static constexpr const char* LINE_WRAP_KEY = "lineWrap";
        /// line wrap title
        static constexpr const char* LINE_WRAP_TITLE = "Line Wrap";
        /// line wrap description
        static constexpr const char* LINE_WRAP_DESC =
            "Enable or disable line wrapping in the log viewer. When enabled, "
            "long lines will be wrapped to fit the width of the log viewer. "
            "When disabled, long lines will be displayed on a single line and "
            "may require horizontal scrolling to view the entire line.";
        /// line wrap default value
        static constexpr bool LINE_WRAP_DEFAULT = false;
    };

    /**
     * @brief Log viewer related settings management
     */
    class LogViewerSettings : public ParamContainerMixin<LogViewerSettings>
    {
       private:
        /// type alias for LogViewerSettingsSchema
        using Schema = LogViewerSettingsSchema;
        /// friend declaration to allow ParamContainerMixin to access private
        /// members of LogViewerSettings, this is necessary because
        /// ParamContainerMixin needs to access the _core member of
        /// LogViewerSettings to implement the functionality for log viewer
        /// settings
        friend ParamContainerMixin<LogViewerSettings>;

        /// The core container for the log viewer settings parameters
        ParamContainer _core;

        /// The interval in seconds for reloading logs
        NumericParam<double> _reloadIntervalSec{
            Schema::RELOAD_INTERVAL_SEC_KEY,
            Schema::RELOAD_INTERVAL_SEC_TITLE,
            Schema::RELOAD_INTERVAL_SEC_DESC
        };

        /// Whether to enable automatic reloading of the log file
        BoolParam _autoReload{
            Schema::AUTO_RELOAD_KEY,
            Schema::AUTO_RELOAD_TITLE,
            Schema::AUTO_RELOAD_DESC
        };

        /// Whether to enable line wrapping in the log viewer
        BoolParam _lineWrap{
            Schema::LINE_WRAP_KEY,
            Schema::LINE_WRAP_TITLE,
            Schema::LINE_WRAP_DESC
        };

       public:
        LogViewerSettings();

        [[nodiscard]] nlohmann::json toJson() const;

        static void fromJson(
            const nlohmann::json& j,
            LogViewerSettings&    settings
        );

        [[nodiscard]] double getReloadIntervalSec() const;
        [[nodiscard]] bool   isAutoReloadEnabled() const;
        [[nodiscard]] bool   isLineWrapEnabled() const;

       private:
        [[nodiscard]] auto _getParams() const&;
        [[nodiscard]] auto _getParams() &;
    };

    /**
     * @brief LogViewerSettings related exceptions
     *
     */
    class LogViewerSettingsException : public MolarTrackerException
    {
       public:
        explicit LogViewerSettingsException(std::string message);

        const char* what() const noexcept override;
    };

}   // namespace settings

#endif   // __LOG_VIEWER_SETTINGS_HPP__
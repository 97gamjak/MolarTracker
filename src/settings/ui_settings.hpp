#ifndef __SETTINGS__UI_SETTINGS_HPP__
#define __SETTINGS__UI_SETTINGS_HPP__

#include "log_viewer_settings.hpp"
#include "params/params.hpp"

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

        nlohmann::json toJson() const;
        static void    fromJson(const nlohmann::json& j, UISettings& settings);

        [[nodiscard]] LogViewerSettings&       getLogViewerSettings();
        [[nodiscard]] const LogViewerSettings& getLogViewerSettings() const;

       private:
        UISettings(std::string key, std::string title, std::string description);

        [[nodiscard]] auto _getParams() const&;
        [[nodiscard]] auto _getParams() &;
    };

}   // namespace settings

#endif   // __SETTINGS__UI_SETTINGS_HPP__
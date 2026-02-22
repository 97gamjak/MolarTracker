#include "ui_settings.hpp"

#include "params/params.hpp"

namespace settings
{
    /**
     * @brief Construct a new UISettings::UISettings object
     *
     */
    UISettings::UISettings()
        : _core{
              UISettingsSchema::UI_SETTINGS_KEY,
              UISettingsSchema::UI_SETTINGS_TITLE,
              UISettingsSchema::UI_SETTINGS_DESC
          }
    {
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
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

    /**
     * @brief Get the DebugSlotsSettings object
     *
     * @return DebugSlotsSettings&
     */
    DebugSlotsSettings& UISettings::getDebugSlotsSettings()
    {
        return _debugSlotsSettings;
    }

    /**
     * @brief Get the DebugSlotsSettings object (const version)
     *
     * @return const DebugSlotsSettings&
     */
    const DebugSlotsSettings& UISettings::getDebugSlotsSettings() const
    {
        return _debugSlotsSettings;
    }

}   // namespace settings
#include "ui_settings.hpp"

namespace settings
{
    class DebugSlotsSettings;   // Forward declaration
    class LogViewerSettings;    // Forward declaration
    class ProfileUISettings;    // Forward declaration
}   // namespace settings

namespace settings
{
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

    /**
     * @brief Get the ProfileUISettings object
     *
     * @return ProfileUISettings&
     */
    ProfileUISettings& UISettings::getProfileUISettings()
    {
        return _profileUISettings;
    }

    /**
     * @brief Get the ProfileUISettings object (const version)
     *
     * @return const ProfileUISettings&
     */
    const ProfileUISettings& UISettings::getProfileUISettings() const
    {
        return _profileUISettings;
    }

}   // namespace settings
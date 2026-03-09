#include "profile_ui_settings.hpp"

#include "config/constants.hpp"

namespace settings
{
    /**
     * @brief Construct a new Profile UI Settings:: Profile UI Settings object
     *
     */
    ProfileUISettings::ProfileUISettings()
    {
        _dialogSize.setDefaults(Schema::DIALOG_SIZE_DEFAULT);
        _dialogSize.setMinValues(Constants::getGlobalMinDialogSize());
    }

    /**
     * @brief Get the window size for the profile dialog
     *
     * @return std::pair<int, int> The window size as a pair of (width, height)
     */
    std::pair<int, int> ProfileUISettings::getDialogSize() const
    {
        return {_dialogSize.x(), _dialogSize.y()};
    }

}   // namespace settings
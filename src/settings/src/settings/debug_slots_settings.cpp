#include "settings/debug_slots_settings.hpp"

#include "config/constants.hpp"
#include "settings/params/params.hpp"

namespace settings
{
    /**
     * @brief Construct a new Debug Slots Settings:: Debug Slots Settings object
     *
     */
    DebugSlotsSettings::DebugSlotsSettings()
    {
        _windowSize.setDefaults(Schema::WINDOW_SIZE_DEFAULT);
        _windowSize.setMinValues(Constants::getGlobalMinDialogSize());
    }

    /**
     * @brief Get the window size for the debug slots dialog
     *
     * @return std::pair<int, int> The window size as a pair of (width, height)
     */
    std::pair<int, int> DebugSlotsSettings::getWindowSize() const
    {
        return {_windowSize.x(), _windowSize.y()};
    }

}   // namespace settings
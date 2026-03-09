#include "debug_slots_settings.hpp"

namespace settings
{
    /**
     * @brief Construct a new Debug Slots Settings:: Debug Slots Settings object
     *
     */
    DebugSlotsSettings::DebugSlotsSettings()
        : _core(Schema::KEY, Schema::TITLE, Schema::DESCRIPTION)
    {
        _windowSize.setDefaults(
            {Schema::WINDOW_SIZE_DEFAULT.first,
             Schema::WINDOW_SIZE_DEFAULT.second}
        );
        _windowSize.setMinValues({1, 1});
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
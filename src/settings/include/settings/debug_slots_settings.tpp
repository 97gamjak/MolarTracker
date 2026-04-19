#ifndef __SETTINGS__INCLUDE__SETTINGS__DEBUG_SLOTS_SETTINGS_TPP__
#define __SETTINGS__INCLUDE__SETTINGS__DEBUG_SLOTS_SETTINGS_TPP__

#include "debug_slots_settings.hpp"

namespace settings
{
    /**
     * @brief private helper function to apply a function to each parameter in
     * the container.
     *
     * @tparam Func
     * @param func
     */
    template <typename Func>
    void DebugSlotsSettings::forEachParam(Func&& func) const
    {
        std::forward<Func>(func)(_windowSize);
    }

    /**
     * @brief private helper function to apply a function to each parameter in
     * the container.
     *
     * @tparam Func
     * @param func
     */
    template <typename Func>
    void DebugSlotsSettings::forEachParam(Func&& func)
    {
        std::forward<Func>(func)(_windowSize);
    }

}   // namespace settings

#endif   // __SETTINGS__INCLUDE__SETTINGS__DEBUG_SLOTS_SETTINGS_TPP__
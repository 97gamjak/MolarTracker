#ifndef __SETTINGS__INCLUDE__SETTINGS__SHORTCUT_SETTINGS_TPP__
#define __SETTINGS__INCLUDE__SETTINGS__SHORTCUT_SETTINGS_TPP__

#include "shortcut_settings.hpp"

namespace settings
{
    /**
     * @brief Applies the given function to each parameter in the
     * ShortcutSettings.
     *
     * @tparam Func The type of the function to apply. It should be callable
     * with a single argument of type MapParam<Shortcut>&.
     * @param func The function to apply to each parameter.
     */
    template <typename Func>
    void ShortcutSettings::forEachParam(Func&& func) const
    {
        std::forward<Func>(func)(_shortcuts);
    }

    /**
     * @brief Applies the given function to each parameter in the
     * ShortcutSettings.
     *
     * @tparam Func The type of the function to apply. It should be callable
     * with a single argument of type MapParam<Shortcut>&.
     * @param func The function to apply to each parameter.
     */
    template <typename Func>
    void ShortcutSettings::forEachParam(Func&& func)
    {
        std::forward<Func>(func)(_shortcuts);
    }
}   // namespace settings

#endif   // __SETTINGS__INCLUDE__SETTINGS__SHORTCUT_SETTINGS_TPP__
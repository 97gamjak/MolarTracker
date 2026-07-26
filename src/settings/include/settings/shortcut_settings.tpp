#ifndef __SETTINGS__INCLUDE__SETTINGS__SHORTCUT_SETTINGS_TPP__
#define __SETTINGS__INCLUDE__SETTINGS__SHORTCUT_SETTINGS_TPP__

#include "shortcut_settings.hpp"

namespace settings
{
    template <typename Func>
    void ShortcutSettings::forEachParam(Func&& func) const
    {
        std::forward<Func>(func)(_shortcuts);
    }

    template <typename Func>
    void ShortcutSettings::forEachParam(Func&& func)
    {
        std::forward<Func>(func)(_shortcuts);
    }
}   // namespace settings

#endif   // __SETTINGS__INCLUDE__SETTINGS__SHORTCUT_SETTINGS_TPP__
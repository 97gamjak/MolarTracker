#ifndef __SETTINGS__INCLUDE__SETTINGS__PROFILE_UI_SETTINGS_TPP__
#define __SETTINGS__INCLUDE__SETTINGS__PROFILE_UI_SETTINGS_TPP__

#include "profile_ui_settings.hpp"

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
    void ProfileUISettings::forEachParam(Func&& func) const
    {
        std::forward<Func>(func)(_dialogSize);
    }

    /**
     * @brief private helper function to apply a function to each parameter in
     * the container.
     *
     * @tparam Func
     * @param func
     */
    template <typename Func>
    void ProfileUISettings::forEachParam(Func&& func)
    {
        std::forward<Func>(func)(_dialogSize);
    }

}   // namespace settings

#endif   // __SETTINGS__INCLUDE__SETTINGS__PROFILE_UI_SETTINGS_TPP__

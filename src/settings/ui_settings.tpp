#ifndef __SETTINGS__UI_SETTINGS_TPP__
#define __SETTINGS__UI_SETTINGS_TPP__

#include "ui_settings.hpp"

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
    void UISettings::_forEachParam(Func&& func) const
    {
        std::forward<Func>(func)(_logViewerSettings);
    }

    /**
     * @brief private helper function to apply a function to each parameter in
     * the container.
     *
     * @tparam Func
     * @param func
     */
    template <typename Func>
    void UISettings::_forEachParam(Func&& func)
    {
        std::forward<Func>(func)(_logViewerSettings);
    }

}   // namespace settings

#endif   // __SETTINGS__UI_SETTINGS_TPP__
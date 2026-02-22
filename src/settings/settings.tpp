#ifndef __SETTINGS__SETTINGS_TPP__
#define __SETTINGS__SETTINGS_TPP__

#include "settings.hpp"

namespace settings
{
    /**
     * @brief private helper function to apply a function to each parameter in
     * the container
     *
     * @tparam Func
     * @param func
     */
    template <typename Func>
    void Settings::_forEachParam(Func&& func) const
    {
        std::forward<Func>(func)(_generalSettings);
        std::forward<Func>(func)(_uiSettings);
        std::forward<Func>(func)(_loggingSettings);
    }

    /**
     * @brief private helper function to apply a function to each parameter in
     * the container
     *
     * @tparam Func
     * @param func
     */
    template <typename Func>
    void Settings::_forEachParam(Func&& func)
    {
        std::forward<Func>(func)(_generalSettings);
        std::forward<Func>(func)(_uiSettings);
        std::forward<Func>(func)(_loggingSettings);
    }

}   // namespace settings

#endif   // __SETTINGS__SETTINGS_TPP__

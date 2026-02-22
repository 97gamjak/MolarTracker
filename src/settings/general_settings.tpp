#ifndef __SETTINGS__GENERAL_SETTINGS_TPP__
#define __SETTINGS__GENERAL_SETTINGS_TPP__

#include "general_settings.hpp"

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
    void GeneralSettings::_forEachParam(Func&& func) const
    {
        std::forward<Func>(func)(_version);
        std::forward<Func>(func)(_defaultProfile);
    }

    /**
     * @brief private helper function to apply a function to each parameter in
     * the container.
     *
     * @tparam Func
     * @param func
     */
    template <typename Func>
    void GeneralSettings::_forEachParam(Func&& func)
    {
        std::forward<Func>(func)(_version);
        std::forward<Func>(func)(_defaultProfile);
    }

}   // namespace settings

#endif   // __SETTINGS__GENERAL_SETTINGS_TPP__
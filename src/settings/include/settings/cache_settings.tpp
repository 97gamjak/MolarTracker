#ifndef __SETTINGS__INCLUDE__SETTINGS__CACHE_SETTINGS_TPP__
#define __SETTINGS__INCLUDE__SETTINGS__CACHE_SETTINGS_TPP__

#include "cache_settings.hpp"

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
    void CacheSettings::forEachParam(Func&& func) const
    {
        std::forward<Func>(func)(_globalMaxCacheSize);
    }

    /**
     * @brief private helper function to apply a function to each parameter in
     * the container.
     *
     * @tparam Func
     * @param func
     */
    template <typename Func>
    void CacheSettings::forEachParam(Func&& func)
    {
        std::forward<Func>(func)(_globalMaxCacheSize);
    }

}   // namespace settings

#endif   // __SETTINGS__INCLUDE__SETTINGS__CACHE_SETTINGS_TPP__
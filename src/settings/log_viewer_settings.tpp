#ifndef __SETTINGS__LOG_VIEWER_SETTINGS_TPP__
#define __SETTINGS__LOG_VIEWER_SETTINGS_TPP__

#include "log_viewer_settings.hpp"

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
    void LogViewerSettings::_forEachParam(Func&& func) const
    {
        std::forward<Func>(func)(_reloadIntervalSec);
        std::forward<Func>(func)(_autoReload);
        std::forward<Func>(func)(_lineWrap);
    }

    /**
     * @brief private helper function to apply a function to each parameter in
     * the container.
     *
     * @tparam Func
     * @param func
     */
    template <typename Func>
    void LogViewerSettings::_forEachParam(Func&& func)
    {
        std::forward<Func>(func)(_reloadIntervalSec);
        std::forward<Func>(func)(_autoReload);
        std::forward<Func>(func)(_lineWrap);
    }

}   // namespace settings

#endif   // __SETTINGS__LOG_VIEWER_SETTINGS_TPP__
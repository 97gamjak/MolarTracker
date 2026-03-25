#ifndef __SETTINGS__LOGGING_SETTINGS_TPP__
#define __SETTINGS__LOGGING_SETTINGS_TPP__

#include "logging_settings.hpp"

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
    void LoggingSettings::_forEachParam(Func&& func) const
    {
        std::forward<Func>(func)(_logDirectory);
        std::forward<Func>(func)(_logFilePrefix);
        std::forward<Func>(func)(_logFileSuffix);
        std::forward<Func>(func)(_maxLogFiles);
        std::forward<Func>(func)(_maxLogFileSizeMB);
        std::forward<Func>(func)(_defaultLogLevel);
    }

    /**
     * @brief private helper function to apply a function to each parameter in
     * the container.
     *
     * @tparam Func
     * @param func
     */
    template <typename Func>
    void LoggingSettings::_forEachParam(Func&& func)
    {
        std::forward<Func>(func)(_logDirectory);
        std::forward<Func>(func)(_logFilePrefix);
        std::forward<Func>(func)(_logFileSuffix);
        std::forward<Func>(func)(_maxLogFiles);
        std::forward<Func>(func)(_maxLogFileSizeMB);
        std::forward<Func>(func)(_defaultLogLevel);
    }

}   // namespace settings

#endif   // __SETTINGS__LOGGING_SETTINGS_TPP__
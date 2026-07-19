#ifndef __SETTINGS__INCLUDE__SETTINGS__BACKUP_SETTINGS_TPP__
#define __SETTINGS__INCLUDE__SETTINGS__BACKUP_SETTINGS_TPP__

#include "backup_settings.hpp"

namespace settings
{
    /**
     * @brief Apply func to each parameter in BackupSettings (const).
     *
     * @tparam Func
     * @param func
     */
    template <typename Func>
    void BackupSettings::forEachParam(Func&& func) const
    {
        std::forward<Func>(func)(_enableBackup);
        std::forward<Func>(func)(_backupDir);
        std::forward<Func>(func)(_recentCount);
        std::forward<Func>(func)(_weeklyCount);
    }

    /**
     * @brief Apply func to each parameter in BackupSettings (non-const).
     *
     * @tparam Func
     * @param func
     */
    template <typename Func>
    void BackupSettings::forEachParam(Func&& func)
    {
        std::forward<Func>(func)(_enableBackup);
        std::forward<Func>(func)(_backupDir);
        std::forward<Func>(func)(_recentCount);
        std::forward<Func>(func)(_weeklyCount);
    }

}   // namespace settings

#endif   // __SETTINGS__INCLUDE__SETTINGS__BACKUP_SETTINGS_TPP__

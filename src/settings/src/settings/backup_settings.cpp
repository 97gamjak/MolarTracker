#include "settings/backup_settings.hpp"

namespace settings
{
    /**
     * @brief Construct a new BackupSettings object, initialising defaults and
     * limits matching the MOLTRACK-157 spec.
     */
    BackupSettings::BackupSettings()
        : _core{
              Schema::BACKUP_SETTINGS_KEY,
              Schema::BACKUP_SETTINGS_TITLE,
              Schema::BACKUP_SETTINGS_DESC
          }
    {
        _enableBackup.setDefault(true);
        _backupDir.setDefault(Schema::BACKUP_DIR_DEFAULT);
        _recentCount.setDefault(Schema::RECENT_COUNT_DEFAULT);
        _recentCount.setMinValue(Schema::RECENT_COUNT_MIN);
        _weeklyCount.setDefault(Schema::WEEKLY_COUNT_DEFAULT);
        _weeklyCount.setMinValue(Schema::WEEKLY_COUNT_MIN);
    }

    /**
     * @brief Return whether automatic backups are enabled.
     *
     * @return bool
     */
    bool BackupSettings::isBackupEnabled() const { return _enableBackup.get(); }

    /**
     * @brief Return the configured backup directory (relative path).
     *
     * @return std::string
     */
    std::string BackupSettings::getBackupDir() const
    {
        return _backupDir.get();
    }

    /**
     * @brief Return the number of most-recent backups to keep.
     *
     * @return std::size_t
     */
    std::size_t BackupSettings::getRecentCount() const
    {
        return _recentCount.get();
    }

    /**
     * @brief Return the number of weekly backup windows.
     *
     * @return std::size_t
     */
    std::size_t BackupSettings::getWeeklyCount() const
    {
        return _weeklyCount.get();
    }

}   // namespace settings

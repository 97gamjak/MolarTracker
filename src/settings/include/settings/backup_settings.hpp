#ifndef __SETTINGS__INCLUDE__SETTINGS__BACKUP_SETTINGS_HPP__
#define __SETTINGS__INCLUDE__SETTINGS__BACKUP_SETTINGS_HPP__

#include <cstddef>
#include <string>

#include "settings/params/params.hpp"

namespace settings
{

    /**
     * @brief Schema constants for backup settings
     */
    class BackupSettingsSchema
    {
       public:
        /******************
         * Backup Settings *
         ******************/

        /// backup settings key
        static constexpr const char* BACKUP_SETTINGS_KEY = "backupSettings";
        /// backup settings title
        static constexpr const char* BACKUP_SETTINGS_TITLE = "Backup Settings";
        /// backup settings description
        static constexpr const char* BACKUP_SETTINGS_DESC =
            "Settings related to the automatic database backup strategy.";

        /*********************
         * Enable Backup     *
         *********************/

        /// enable backup key
        static constexpr const char* ENABLE_BACKUP_KEY = "enableBackup";
        /// enable backup title
        static constexpr const char* ENABLE_BACKUP_TITLE = "Enable Backup";
        /// enable backup description
        static constexpr const char* ENABLE_BACKUP_DESC =
            "When enabled, a backup of the database is created on every "
            "application startup.";

        /*********************
         * Backup Directory  *
         *********************/

        /// backup directory key
        static constexpr const char* BACKUP_DIR_KEY = "backupDir";
        /// backup directory title
        static constexpr const char* BACKUP_DIR_TITLE = "Backup Directory";
        /// backup directory description
        static constexpr const char* BACKUP_DIR_DESC =
            "Directory where database backups are stored. Relative paths "
            "are resolved from the application data directory.";
        /// backup directory default value
        static constexpr const char* BACKUP_DIR_DEFAULT = "backups";

        /*********************
         * Recent Count      *
         *********************/

        /// recent backup count key
        static constexpr const char* RECENT_COUNT_KEY = "recentCount";
        /// recent backup count title
        static constexpr const char* RECENT_COUNT_TITLE =
            "Recent Backups to Keep";
        /// recent backup count description
        static constexpr const char* RECENT_COUNT_DESC =
            "Number of most-recent backups to keep unconditionally.";
        /// recent backup count default
        static constexpr std::size_t RECENT_COUNT_DEFAULT = 5;
        /// recent backup count minimum
        static constexpr std::size_t RECENT_COUNT_MIN = 1;

        /*********************
         * Weekly Count      *
         *********************/

        /// weekly backup count key
        static constexpr const char* WEEKLY_COUNT_KEY = "weeklyCount";
        /// weekly backup count title
        static constexpr const char* WEEKLY_COUNT_TITLE =
            "Weekly Backup Windows";
        /// weekly backup count description
        static constexpr const char* WEEKLY_COUNT_DESC =
            "Number of calendar weeks beyond the recent set for which one "
            "backup per week is retained.";
        /// weekly backup count default
        static constexpr std::size_t WEEKLY_COUNT_DEFAULT = 4;
        /// weekly backup count minimum
        static constexpr std::size_t WEEKLY_COUNT_MIN = 0;
    };

    /**
     * @brief Backup-related settings management
     */
    class BackupSettings : public ParamContainerMixin<BackupSettings>
    {
       private:
        /// type alias for schema
        using Schema = BackupSettingsSchema;

        /// Allow ParamContainerMixin to access private members
        friend ParamContainerMixin<BackupSettings>;

        /// Core container for backup settings parameters
        ParamContainer _core;

        /// Whether backups are enabled
        BoolParam _enableBackup{
            Schema::ENABLE_BACKUP_KEY,
            Schema::ENABLE_BACKUP_TITLE,
            Schema::ENABLE_BACKUP_DESC
        };

        /// Relative backup directory path
        StringParam _backupDir{
            Schema::BACKUP_DIR_KEY,
            Schema::BACKUP_DIR_TITLE,
            Schema::BACKUP_DIR_DESC
        };

        /// Number of most-recent backups to keep
        NumericParam<std::size_t> _recentCount{
            Schema::RECENT_COUNT_KEY,
            Schema::RECENT_COUNT_TITLE,
            Schema::RECENT_COUNT_DESC
        };

        /// Number of weekly backup windows to keep
        NumericParam<std::size_t> _weeklyCount{
            Schema::WEEKLY_COUNT_KEY,
            Schema::WEEKLY_COUNT_TITLE,
            Schema::WEEKLY_COUNT_DESC
        };

       public:
        BackupSettings();

        [[nodiscard]] bool        isBackupEnabled() const;
        [[nodiscard]] std::string getBackupDir() const;
        [[nodiscard]] std::size_t getRecentCount() const;
        [[nodiscard]] std::size_t getWeeklyCount() const;

        template <typename Func>
        void forEachParam(Func&& func) const;
        template <typename Func>
        void forEachParam(Func&& func);
    };

}   // namespace settings

#ifndef __SETTINGS__INCLUDE__SETTINGS__BACKUP_SETTINGS_TPP__
#include "backup_settings.tpp"
#endif

#endif   // __SETTINGS__INCLUDE__SETTINGS__BACKUP_SETTINGS_HPP__

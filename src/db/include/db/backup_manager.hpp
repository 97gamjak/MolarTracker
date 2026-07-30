#ifndef __DB__INCLUDE__DB__BACKUP_MANAGER_HPP__
#define __DB__INCLUDE__DB__BACKUP_MANAGER_HPP__

#include <cstddef>
#include <string>
#include <vector>

namespace settings
{
    class BackupSettings;   // Forward declaration
}   // namespace settings

class Timestamp;   // Forward declaration

namespace db
{
    class Database;   // Forward declaration

    /**
     * @brief Manages timestamped SQLite backups with a tiered retention policy.
     *
     *   - Recent:  keep the N most recent backups (default 5)
     *   - Weekly:  for each of W calendar weeks beyond the recent set, keep the
     *              newest backup from that week (default 4 weeks)
     *   - Monthly: for each calendar month older than the weekly window, keep
     *              the newest backup from that month (unbounded)
     *
     */
    class BackupManager
    {
       public:
        /**
         * @brief Retention policy controlling how many backups are kept per
         * tier.
         */
        struct RetentionPolicy
        {
            /// Number of most-recent backups to keep unconditionally
            std::size_t recentCount;
            /// Number of calendar weeks in the weekly tier
            std::size_t weeklyCount;
        };

        static void createBackup(
            Database&                       db,
            const settings::BackupSettings& backupSettings
        );

        [[nodiscard]]
        static std::vector<std::string> listBackups(
            const settings::BackupSettings& backupSettings
        );
    };

}   // namespace db

#endif   // __DB__INCLUDE__DB__BACKUP_MANAGER_HPP__

#ifndef __DB__INCLUDE__DB__BACKUP_MANAGER_HPP__
#define __DB__INCLUDE__DB__BACKUP_MANAGER_HPP__

#include <chrono>
#include <cstddef>
#include <filesystem>
#include <optional>
#include <vector>

namespace db
{
    class Database;   // Forward declaration

    /**
     * @brief Manages timestamped SQLite backups with a tiered retention policy.
     *
     * Backup files are named `molartracker_YYYYMMDD_HHMMSS.db` and stored in a
     * configurable directory. Pruning applies three tiers:
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
            Database&                    db,
            const std::filesystem::path& backupDir,
            const RetentionPolicy&       policy
        );

        [[nodiscard]]
        static std::vector<std::filesystem::path> listBackups(
            const std::filesystem::path& backupDir
        );

       private:
        static void _prune(
            const std::vector<std::filesystem::path>& sorted,
            const RetentionPolicy&                    policy
        );

        [[nodiscard]]
        static std::
            optional<std::chrono::system_clock::time_point> _parseTimestamp(
                const std::filesystem::path& p
            );
    };

}   // namespace db

#endif   // __DB__INCLUDE__DB__BACKUP_MANAGER_HPP__

#include "db/backup_manager.hpp"

#include <sqlite3.h>

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "config/constants/constants.hpp"
#include "db/database.hpp"
#include "db/db_exception.hpp"
#include "logging/log_macros.hpp"
#include "settings/backup_settings.hpp"
#include "utils/timestamp.hpp"

REGISTER_LOG_CATEGORY("DB.BackupManager");

namespace db
{

    namespace
    {
        /**
         * @brief Opens a new SQLite connection at the given path (read/write,
         * create if absent). Caller must close the handle.
         */
        sqlite3* _openDest(const std::string& path)
        {
            sqlite3*   handle = nullptr;
            const auto result = sqlite3_open_v2(
                path.c_str(),
                &handle,
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                nullptr
            );
            if (result != SQLITE_OK)
                throw SqliteError("BackupManager: failed to open dest " + path);
            return handle;
        }

        std::optional<Timestamp> _parseTimestamp(
            const std::filesystem::path& path
        )
        {
            const auto fileName = path.filename().string();
            return Timestamp::fromFileSafe(
                fileName,
                Constants::getFilePrefix(),
                Constants::getDatabaseFileExtension()
            );
        }

        /**
         * @brief Apply the tiered retention policy, deleting files that fall
         * outside all tiers.
         *
         * Tier order (newest first):
         *   1. Recent — keep `policy.recentCount` files unconditionally
         *   2. Weekly — keep one file per calendar week for
         * `policy.weeklyCount` calendar weeks beyond the recent set
         *   3. Monthly — keep one file per calendar month for all older months
         *
         * @param sorted Backup paths sorted newest first
         * @param policy Retention configuration
         */
        void _prune(
            const std::vector<std::filesystem::path>& sorted,
            const BackupManager::RetentionPolicy&     policy
        )
        {
            std::set<std::filesystem::path> keep;

            // ── Tier 1: N most recent
            // ──────────────────────────────────────────
            const auto recentEnd = std::min(policy.recentCount, sorted.size());
            for (std::size_t i = 0; i < recentEnd; ++i)
                keep.insert(sorted[i]);

            // ── Tier 2: Weekly
            // ──────────────────────────────────────────────── For each ISO
            // year-week key in the first `weeklyCount` distinct weeks beyond
            // the recent set, keep the newest (first in sorted order).
            std::map<std::pair<int, unsigned>, std::filesystem::path> weekMap;
            for (std::size_t i = recentEnd; i < sorted.size(); ++i)
            {
                const auto timeStamp = _parseTimestamp(sorted[i]);
                if (!timeStamp.has_value())
                    continue;

                const auto key =
                    std::make_pair(timeStamp->year(), timeStamp->week());

                if (!weekMap.contains(key))
                    weekMap[key] = sorted[i];
            }

            std::size_t weeksAdded = 0;
            for (const auto& [key, path] : weekMap)
            {
                if (weeksAdded >= policy.weeklyCount)
                    break;
                keep.insert(path);
                ++weeksAdded;
            }

            // ── Tier 3: Monthly (unbounded)
            // ────────────────────────────────────
            std::map<std::pair<int, unsigned>, std::filesystem::path> monthMap;
            for (std::size_t i = recentEnd; i < sorted.size(); ++i)
            {
                const auto timeStamp = _parseTimestamp(sorted[i]);
                if (!timeStamp.has_value())
                    continue;
                if (keep.contains(sorted[i]))
                    continue;

                const auto key =
                    std::make_pair(timeStamp->year(), timeStamp->month());
                if (!monthMap.contains(key))
                    monthMap[key] = sorted[i];
            }
            for (const auto& [key, path] : monthMap)
                keep.insert(path);

            // ── Delete everything not in `keep`
            // ────────────────────────────────
            for (const auto& path : sorted)
            {
                if (keep.contains(path))
                    continue;
                LOG_INFO("Pruning old backup: " + path.string());
                std::filesystem::remove(path);
            }
        }

        /**
         * @brief Scan the backup directory for files matching the naming
         * convention and return them sorted newest first.
         *
         * @param backupDir Directory to scan
         * @return Paths sorted newest first
         */
        std::vector<std::filesystem::path> _listBackups(
            const settings::BackupSettings& backupSettings
        )
        {
            if (!backupSettings.isBackupEnabled())
                return {};

            const auto backupDir = backupSettings.getBackupPath();

            if (!std::filesystem::exists(backupDir))
                return {};

            std::vector<std::filesystem::path> result;
            for (const auto& entry :
                 std::filesystem::directory_iterator(backupDir))
            {
                if (!entry.is_regular_file())
                    continue;

                const auto& path = entry.path();
                const auto  stem = path.stem().string();

                const auto& filePrefix = Constants::getFilePrefix();
                if (!stem.starts_with(filePrefix))
                {
                    LOG_WARNING(
                        std::format(
                            "Found backup file that does not start with "
                            "configured file prefix: {} -> {}",
                            filePrefix,
                            path.string()
                        )
                    );
                    continue;
                }

                if (path.extension() != Constants::getDatabaseFileExtension())
                {
                    LOG_WARNING(
                        std::format(
                            "Found backup file that does not have the expected "
                            "extension: {} -> {}",
                            Constants::getDatabaseFileExtension(),
                            path.string()
                        )
                    );
                    continue;
                }

                if (_parseTimestamp(path).has_value())
                    result.push_back(path);
                else
                {
                    LOG_WARNING(
                        std::format(
                            "Found backup file with invalid timestamp: {}",
                            path.string()
                        )
                    );
                }
            }

            std::ranges::sort(
                result,
                [](const auto& left, const auto& right)
                {
                    return _parseTimestamp(left).value_or(Timestamp::Null()) >
                           _parseTimestamp(right).value_or(Timestamp::Null());
                }
            );

            return result;
        }
    }   // namespace

    /**
     * @brief Create a timestamped backup copy using `sqlite3_backup_*` and
     * then apply the tiered retention policy.
     *
     * @param db        Live, open source database
     * @param backupSettings Settings controlling backup directory and retention
     * policy
     */
    void BackupManager::createBackup(
        Database&                       db,
        const settings::BackupSettings& backupSettings
    )
    {
        if (!backupSettings.isBackupEnabled())
        {
            LOG_INFO("BackupManager: backups are disabled, skipping");
            return;
        }

        const auto dir = backupSettings.getBackupPath();
        std::filesystem::create_directories(dir);

        const auto destName = Constants::getFilePrefix() + "_" +
                              Timestamp().fileSafe() +
                              Constants::getDatabaseFileExtension();

        const auto destPath = (dir / destName).string();

        LOG_INFO("Creating database backup: " + destPath);

        sqlite3* destHandle = _openDest(destPath);

        sqlite3_backup* backup =
            sqlite3_backup_init(destHandle, "main", db.nativeHandle(), "main");

        if (backup == nullptr)
        {
            sqlite3_close(destHandle);
            throw SqliteError(
                "BackupManager: sqlite3_backup_init failed for " + destPath
            );
        }

        sqlite3_backup_step(backup, -1);
        sqlite3_backup_finish(backup);
        sqlite3_close(destHandle);

        LOG_INFO("Backup complete: " + destPath);

        const auto all = _listBackups(backupSettings);
        _prune(
            all,
            BackupManager::RetentionPolicy{
                .recentCount = backupSettings.getRecentCount(),
                .weeklyCount = backupSettings.getWeeklyCount()
            }
        );
    }

    /**
     * @brief Scan the backup directory for files matching the naming convention
     * and return them sorted newest first.
     *
     * @param backupSettings Settings controlling backup directory and retention
     * policy
     * @return Paths sorted newest first
     */
    std::vector<std::string> BackupManager::listBackups(
        const settings::BackupSettings& backupSettings
    )
    {
        auto backups = _listBackups(backupSettings) |
                       std::views::transform([](const auto& path)
                                             { return path.string(); });

        return {backups.begin(), backups.end()};
    }

}   // namespace db

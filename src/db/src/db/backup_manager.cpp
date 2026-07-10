#include "db/backup_manager.hpp"

#include <sqlite3.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "config/constants.hpp"
#include "db/database.hpp"
#include "db/db_exception.hpp"
#include "logging/log_macros.hpp"
#include "utils/timestamp.hpp"

REGISTER_LOG_CATEGORY("DB.BackupManager");

using std::chrono::year_month_day;

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
    }   // namespace

    /**
     * @brief Create a timestamped backup copy using `sqlite3_backup_*` and
     * then apply the tiered retention policy.
     *
     * @param db        Live, open source database
     * @param backupDir Target directory (created if absent)
     * @param policy    Retention tier configuration
     */
    void BackupManager::createBackup(
        Database&                    db,
        const std::filesystem::path& backupDir,
        const RetentionPolicy&       policy
    )
    {
        std::filesystem::create_directories(backupDir);

        const auto destName = Constants::getFilePrefix() +
                              Timestamp().fileSafe() + "." +
                              Constants::getDatabaseFileExtension();

        const auto destPath = (backupDir / destName).string();

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

        const auto all = listBackups(backupDir);
        _prune(all, policy);
    }

    /**
     * @brief Scan the backup directory for files matching the naming convention
     * and return them sorted newest first.
     *
     * @param backupDir Directory to scan
     * @return Paths sorted newest first
     */
    std::vector<std::filesystem::path> BackupManager::listBackups(
        const std::filesystem::path& backupDir
    )
    {
        if (!std::filesystem::exists(backupDir))
            return {};

        std::vector<std::filesystem::path> result;
        for (const auto& entry : std::filesystem::directory_iterator(backupDir))
        {
            if (!entry.is_regular_file())
                continue;

            const auto& path = entry.path();
            const auto  stem = path.stem().string();

            if (!stem.starts_with(Constants::getFilePrefix()))
                continue;

            if (path.extension() != Constants::getDatabaseFileExtension())
                continue;

            if (_parseTimestamp(path).has_value())
                result.push_back(path);
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

    /**
     * @brief Apply the tiered retention policy, deleting files that fall
     * outside all tiers.
     *
     * Tier order (newest first):
     *   1. Recent — keep `policy.recentCount` files unconditionally
     *   2. Weekly — keep one file per calendar week for `policy.weeklyCount`
     *      calendar weeks beyond the recent set
     *   3. Monthly — keep one file per calendar month for all older months
     *
     * @param sorted Backup paths sorted newest first
     * @param policy Retention configuration
     */
    void BackupManager::_prune(
        const std::vector<std::filesystem::path>& sorted,
        const RetentionPolicy&                    policy
    )
    {
        std::set<std::filesystem::path> keep;

        // ── Tier 1: N most recent ──────────────────────────────────────────
        const auto recentEnd = std::min(policy.recentCount, sorted.size());
        for (std::size_t i = 0; i < recentEnd; ++i)
            keep.insert(sorted[i]);

        // ── Tier 2: Weekly ────────────────────────────────────────────────
        // For each ISO year-week key in the first `weeklyCount` distinct weeks
        // beyond the recent set, keep the newest (first in sorted order).
        std::map<std::pair<int, unsigned>, std::filesystem::path> weekMap;
        for (std::size_t i = recentEnd; i < sorted.size(); ++i)
        {
            const auto timeStamp = _parseTimestamp(sorted[i]);
            if (!timeStamp.has_value())
                continue;

            const auto           days = floor<std::chrono::days>(*timeStamp);
            const year_month_day ymd{days};

            // Simple week number: days since epoch / 7
            const auto weekNum = static_cast<unsigned>(
                static_cast<std::int64_t>(days.time_since_epoch().count()) / 7
            );

            const auto key =
                std::make_pair(static_cast<int>(ymd.year()), weekNum);

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

        // ── Tier 3: Monthly (unbounded) ────────────────────────────────────
        std::map<std::pair<int, unsigned>, std::filesystem::path> monthMap;
        for (std::size_t i = recentEnd; i < sorted.size(); ++i)
        {
            const auto timeStamp = _parseTimestamp(sorted[i]);
            if (!timeStamp.has_value())
                continue;
            if (keep.contains(sorted[i]))
                continue;

            const auto           days = floor<std::chrono::days>(*timeStamp);
            const year_month_day ymd{days};
            const auto           key = std::make_pair(
                static_cast<int>(ymd.year()),
                static_cast<unsigned>(ymd.month())
            );
            if (!monthMap.contains(key))
                monthMap[key] = sorted[i];
        }
        for (const auto& [key, path] : monthMap)
            keep.insert(path);

        // ── Delete everything not in `keep` ────────────────────────────────
        for (const auto& path : sorted)
        {
            if (keep.contains(path))
                continue;
            LOG_INFO("Pruning old backup: " + path.string());
            std::filesystem::remove(path);
        }
    }

    /**
     * @brief Parse the timestamp embedded in a backup filename.
     *
     * @param p Path to parse
     * @return Parsed time point, or std::nullopt if the name does not match
     */
    std::optional<Timestamp> BackupManager::_parseTimestamp(
        const std::filesystem::path& path
    )
    {
        const auto stem = path.stem().string();

        if (!stem.starts_with(Constants::getFilePrefix()))
            return std::nullopt;

        return Timestamp::fromHumanReadable(
            stem.substr(Constants::getFilePrefix().size())
        );
    }

}   // namespace db

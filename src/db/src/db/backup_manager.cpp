#include "db/backup_manager.hpp"

#include <sqlite3.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <filesystem>
#include <format>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "db/database.hpp"
#include "db/db_exception.hpp"
#include "logging/log_macros.hpp"
#include "utils/timestamp.hpp"

REGISTER_LOG_CATEGORY("DB.BackupManager");

namespace db
{

    namespace
    {
        constexpr std::string_view k_prefix = "molartracker_";
        constexpr std::string_view k_suffix = ".db";

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
        const std::filesystem::path& backupDir
    )
    {
        createBackup(db, backupDir, RetentionPolicy{});
    }

    void BackupManager::createBackup(
        Database&                    db,
        const std::filesystem::path& backupDir,
        const RetentionPolicy&       policy
    )
    {
        std::filesystem::create_directories(backupDir);

        const auto destName = std::string{k_prefix} + Timestamp().fileSafe() +
                              std::string{k_suffix};
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
            const auto& p    = entry.path();
            const auto  stem = p.stem().string();
            if (!stem.starts_with(k_prefix))
                continue;
            if (p.extension() != k_suffix)
                continue;
            if (_parseTimestamp(p).has_value())
                result.push_back(p);
        }

        std::ranges::sort(
            result,
            [](const auto& a, const auto& b)
            {
                return _parseTimestamp(a).value_or(
                           std::chrono::system_clock::time_point{}
                       ) >
                       _parseTimestamp(b).value_or(
                           std::chrono::system_clock::time_point{}
                       );
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
        using namespace std::chrono;

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
            const auto ts = _parseTimestamp(sorted[i]);
            if (!ts.has_value())
                continue;

            const auto           days = floor<std::chrono::days>(*ts);
            const year_month_day ymd{days};

            // Simple week number: days since epoch / 7
            const auto weekNum = static_cast<unsigned>(
                static_cast<long long>(days.time_since_epoch().count()) / 7
            );

            const auto key =
                std::make_pair(static_cast<int>(ymd.year()), weekNum);

            if (weekMap.find(key) == weekMap.end())
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
            const auto ts = _parseTimestamp(sorted[i]);
            if (!ts.has_value())
                continue;
            if (keep.contains(sorted[i]))
                continue;

            const auto           days = floor<std::chrono::days>(*ts);
            const year_month_day ymd{days};
            const auto           key = std::make_pair(
                static_cast<int>(ymd.year()),
                static_cast<unsigned>(ymd.month())
            );
            if (monthMap.find(key) == monthMap.end())
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
     * @brief Parse the timestamp embedded in a backup filename of the form
     * `molartracker_YYYYMMDD_HHMMSS.db`.
     *
     * @param p Path to parse
     * @return Parsed time point, or std::nullopt if the name does not match
     */
    std::optional<std::chrono::system_clock::time_point> BackupManager::
        _parseTimestamp(const std::filesystem::path& p)
    {
        const auto stem = p.stem().string();

        if (!stem.starts_with(k_prefix))
            return std::nullopt;

        // Extract the timestamp portion: after prefix, expect
        // YYYYMMDD_HHMMSS (15 chars)
        const auto tsStr = stem.substr(k_prefix.size());   // "YYYYMMDD_HHMMSS"
        if (tsStr.size() < 15)
            return std::nullopt;

        // YYYYMMDD_HHMMSS
        int year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,cert-err34-c)
        if (std::sscanf(
                tsStr.c_str(),
                "%4d%2d%2d_%2d%2d%2d",
                &year,
                &month,
                &day,
                &hour,
                &min,
                &sec
            ) != 6)
            return std::nullopt;

        using namespace std::chrono;
        const auto ymd = year_month_day{
            std::chrono::year{year},
            std::chrono::month{static_cast<unsigned>(month)},
            std::chrono::day{static_cast<unsigned>(day)}
        };
        if (!ymd.ok())
            return std::nullopt;

        const auto tp =
            sys_days{ymd} + hours{hour} + minutes{min} + seconds{sec};
        return tp;
    }

}   // namespace db

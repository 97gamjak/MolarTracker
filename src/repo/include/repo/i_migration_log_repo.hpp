#ifndef __REPO__INCLUDE__REPO__I_MIGRATION_LOG_REPO_HPP__
#define __REPO__INCLUDE__REPO__I_MIGRATION_LOG_REPO_HPP__

#include <cstdint>
#include <string>
#include <vector>

namespace repo
{

    /**
     * @brief One applied database migration step, as recorded in the
     * migration_log table
     *
     */
    struct MigrationLogEntry
    {
        /// The schema version this step was applied from
        std::int64_t fromVersion;
        /// The schema version this step was applied to
        std::int64_t toVersion;
        /// The app release version this step shipped with (e.g. "0.3.0")
        std::string releaseVersion;
        /// When this step was applied, as an ISO-8601 UTC string
        std::string appliedAt;
    };

    /**
     * @brief Interface for the migration log repository
     *
     */
    class IMigrationLogRepo
    {
       public:
        virtual ~IMigrationLogRepo() = default;

        /**
         * @brief Get every recorded migration log entry, ordered from
         * oldest to newest
         *
         * @return std::vector<MigrationLogEntry>
         */
        [[nodiscard]]
        virtual std::vector<MigrationLogEntry> getAll() = 0;

        /**
         * @brief Get the database's current schema version (SQLite's
         * `PRAGMA user_version`)
         *
         * @return std::int64_t
         */
        [[nodiscard]]
        virtual std::int64_t getCurrentDbVersion() = 0;

        /**
         * @brief Get the schema version this app version migrates to
         *
         * @return std::int64_t
         */
        [[nodiscard]]
        virtual std::int64_t getTargetDbVersion() = 0;
    };

}   // namespace repo

#endif   // __REPO__INCLUDE__REPO__I_MIGRATION_LOG_REPO_HPP__

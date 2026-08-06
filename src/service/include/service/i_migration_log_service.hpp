#ifndef __SERVICE__INCLUDE__SERVICE__I_MIGRATION_LOG_SERVICE_HPP__
#define __SERVICE__INCLUDE__SERVICE__I_MIGRATION_LOG_SERVICE_HPP__

#include <cstdint>
#include <string>
#include <vector>

namespace service
{

    /**
     * @brief One applied database migration step, for display purposes
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
     * @brief Interface for the migration log service
     *
     */
    class IMigrationLogService
    {
       public:
        virtual ~IMigrationLogService() = default;

        /**
         * @brief Get every recorded migration log entry, ordered from
         * oldest to newest
         *
         * @return std::vector<MigrationLogEntry>
         */
        [[nodiscard]]
        virtual std::vector<MigrationLogEntry> getAll() const = 0;

        /**
         * @brief Get the database's current schema version
         *
         * @return std::int64_t
         */
        [[nodiscard]]
        virtual std::int64_t getCurrentDbVersion() const = 0;

        /**
         * @brief Get the schema version this app version migrates to
         *
         * @return std::int64_t
         */
        [[nodiscard]]
        virtual std::int64_t getTargetDbVersion() const = 0;
    };

}   // namespace service

#endif   // __SERVICE__INCLUDE__SERVICE__I_MIGRATION_LOG_SERVICE_HPP__

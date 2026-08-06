#include "migration_log_service.hpp"

#include "repo/i_migration_log_repo.hpp"

namespace service
{

    /**
     * @brief Construct a new Migration Log Service:: Migration Log Service
     * object
     *
     * @param migrationLogRepo
     */
    MigrationLogService::MigrationLogService(
        const std::shared_ptr<repo::IMigrationLogRepo>& migrationLogRepo
    )
        : _migrationLogRepo(migrationLogRepo)
    {
    }

    /**
     * @brief Get every recorded migration log entry, ordered from oldest to
     * newest
     *
     * @return std::vector<MigrationLogEntry>
     */
    std::vector<MigrationLogEntry> MigrationLogService::getAll() const
    {
        std::vector<MigrationLogEntry> entries;
        for (const auto& row : _migrationLogRepo->getAll())
        {
            entries.push_back(
                MigrationLogEntry{
                    .fromVersion    = row.fromVersion,
                    .toVersion      = row.toVersion,
                    .releaseVersion = row.releaseVersion,
                    .appliedAt      = row.appliedAt,
                }
            );
        }

        return entries;
    }

    /**
     * @brief Get the database's current schema version
     *
     * @return std::int64_t
     */
    std::int64_t MigrationLogService::getCurrentDbVersion() const
    {
        return _migrationLogRepo->getCurrentDbVersion();
    }

    /**
     * @brief Get the schema version this app version migrates to
     *
     * @return std::int64_t
     */
    std::int64_t MigrationLogService::getTargetDbVersion() const
    {
        return _migrationLogRepo->getTargetDbVersion();
    }

}   // namespace service

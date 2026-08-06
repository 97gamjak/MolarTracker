#include "migration_log_repo.hpp"

#include "orm/crud.hpp"
#include "orm/query_options.hpp"
#include "repo/migration/migration_runner.hpp"
#include "sql_models/migration_log_row.hpp"

namespace repo
{

    /**
     * @brief Get every recorded migration log entry, ordered from oldest to
     * newest
     *
     * @return std::vector<MigrationLogEntry>
     */
    std::vector<MigrationLogEntry> MigrationLogRepo::getAll()
    {
        auto query = orm::Query{}.orderBy<MigrationLogRow::idField>(true);

        const auto rows = _getCrud().get<MigrationLogRow>(_getDb(), query);

        std::vector<MigrationLogEntry> entries;
        entries.reserve(rows.size());

        for (const auto& row : rows)
        {
            entries.push_back(
                MigrationLogEntry{
                    .fromVersion    = row.fromVersion.value(),
                    .toVersion      = row.toVersion.value(),
                    .releaseVersion = row.releaseVersion.value(),
                    .appliedAt      = row.appliedAt.value(),
                }
            );
        }

        return entries;
    }

    /**
     * @brief Get the database's current schema version (SQLite's
     * `PRAGMA user_version`)
     *
     * @return std::int64_t
     */
    std::int64_t MigrationLogRepo::getCurrentDbVersion()
    {
        return _getDb().queryInt("PRAGMA user_version");
    }

    /**
     * @brief Get the schema version this app version migrates to
     *
     * @return std::int64_t
     */
    std::int64_t MigrationLogRepo::getTargetDbVersion()
    {
        return static_cast<std::int64_t>(MigrationRunner::getTargetVersion());
    }

}   // namespace repo

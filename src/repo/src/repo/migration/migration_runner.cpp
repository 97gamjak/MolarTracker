#include "migration_runner.hpp"

#include "db/database.hpp"
#include "db/transaction.hpp"
#include "logging/log_macros.hpp"
#include "migration.hpp"
#include "repo/exceptions.hpp"

REGISTER_LOG_CATEGORY("Repo.Migration");

namespace repo
{
    /**
     * @brief Construct a new MigrationRunner object
     *
     * @param db The database to migrate
     */
    MigrationRunner::MigrationRunner(db::Database& db)
        : _migrations(0, DB_VERSION)
    {
        try
        {
            migrate(db);
        }
        catch (const std::exception& e)
        {
            throw MigrationException(e.what(), db.getDBPath());
        }
    }

    /**
     * @brief determine last db version and apply all migrations needed
     *
     * @param db
     */
    void MigrationRunner::migrate(db::Database& db)
    {
        const auto dbVersion =
            static_cast<size_t>(db.queryInt("PRAGMA user_version"));

        LOG_INFO(
            "Applying migrations: " + std::to_string(dbVersion) + " -> " +
            std::to_string(DB_VERSION)
        );

        // Backup is now handled by RepoContainer before migration runs.

        _migrations = Migrations{dbVersion, DB_VERSION};

        db::Transaction transaction{db};

        _migrations.migrate(db);

        const auto stmt   = std::format("PRAGMA user_version = {}", DB_VERSION);
        const auto result = db.execute(stmt);
        if (!result)
        {
            throw MigrationException(
                "Failed to set user_version to " + std::to_string(DB_VERSION) +
                    " after migration with SQL: " + stmt +
                    " | Error: " + result.error().toString(),
                db.getDBPath()
            );
        }

        const auto commitResult = transaction.commit();
        if (!commitResult)
        {
            throw MigrationException(
                "Failed to commit transaction after migration: " +
                    commitResult.error().toString(),
                db.getDBPath()
            );
        }
    }

}   // namespace repo

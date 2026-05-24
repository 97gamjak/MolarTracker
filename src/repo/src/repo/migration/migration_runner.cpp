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

        db.makeBackup();

        _migrations = Migrations{dbVersion, DB_VERSION};

        db::Transaction transaction{db};

        _migrations.migrate(db);

        db.execute("PRAGMA user_version = " + std::to_string(DB_VERSION));

        transaction.commit();
    }

}   // namespace repo

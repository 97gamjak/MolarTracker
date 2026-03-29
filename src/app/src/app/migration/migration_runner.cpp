#include "app/migration/migration_runner.hpp"

#include "app/migration/migration.hpp"
#include "db/database.hpp"
#include "db/transaction.hpp"

namespace app
{
    /**
     * @brief Construct a new MigrationRunner object
     *
     * @param db The database to migrate
     */
    MigrationRunner::MigrationRunner(db::Database& db)
        : _migrations(0, DB_VERSION)
    {
        migrate(db);
    }

    void MigrationRunner::migrate(db::Database& db)
    {
        const auto dbVersion =
            static_cast<size_t>(db.queryInt("PRAGMA user_version"));

        _migrations = Migrations{dbVersion, DB_VERSION};

        db::Transaction transaction{db};

        _migrations.migrate(db);

        db.execute("PRAGMA user_version = " + std::to_string(DB_VERSION));

        transaction.commit();
    }

}   // namespace app

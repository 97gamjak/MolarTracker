#include "app/migration/migration_runner.hpp"

#include "app/migration/migration.hpp"

namespace app
{
    MigrationRunner::MigrationRunner(db::Database& db)
        : _migrationStates(Migration::migrate(db))
    {
    }

}   // namespace app

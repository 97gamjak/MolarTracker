#include "app/migration/migration.hpp"

#include <cstddef>
#include <memory>

#include "app/migration/migration_state.hpp"
#include "db/database.hpp"
#include "db/transaction.hpp"

namespace app
{
    Migration::Migration()
    {
        static_assert(
            _migrations.size() + 1 == MigrationVersion,
            "Migration version mismatch"
        );
    }

    MigrationStates Migration::migrate(db::Database& db)
    {
        const auto currentVersion =
            static_cast<size_t>(db.queryInt("PRAGMA user_version"));

        MigrationStates migrationStates{currentVersion, MigrationVersion};

        db::Transaction transaction{db};

        for (std::size_t i = currentVersion + 1; i < MigrationVersion; ++i)
        {
            _migrations[i](db);
            MigrationState state{i};
            migrationStates.addMigration(state);
        }

        db.execute("PRAGMA user_version = " + std::to_string(MigrationVersion));

        transaction.commit();

        return migrationStates;
    }

}   // namespace app

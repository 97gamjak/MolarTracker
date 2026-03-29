#include "app/migration/migration.hpp"

#include <cassert>

#include "app/migration/single_migration.hpp"
#include "db/database.hpp"
#include "sql_models/account_row.hpp"
#include "sql_models/profile_row.hpp"

namespace app
{

    /**
     * @brief Construct a new Migration object
     *
     * @param fromVersion The version the migration is being applied from
     */
    Migration::Migration(std::size_t fromVersion) : _fromVersion(fromVersion) {}

    /**
     * @brief apply all single migrations that are part of this Migration
     * container
     *
     * @param db
     */
    void Migration::migrate(db::Database& db) const
    {
        for (const auto& migration : _migrations)
            migration->applyMigration(db);
    }

    /**
     * @brief add a single migration to the migration container
     *
     * @param migration
     */
    void Migration::addMigration(std::unique_ptr<SingleMigration> migration)
    {
        _migrations.push_back(std::move(migration));
    }

    /**
     * @brief Construct a new Migrations object
     *
     * @param fromVersion The version the migration is being applied from
     * @param toVersion The version the migration is being applied to
     */
    Migrations::Migrations(std::size_t fromVersion, std::size_t toVersion)
        : _fromVersion(fromVersion), _toVersion(toVersion)
    {
        assert(fromVersion < toVersion);
        assert(_migrations.empty());

        // add migrations
        Migration migration(0);
        migration.addMigration(
            std::make_unique<CreateTableMigration<ProfileRow>>()
        );
        migration.addMigration(
            std::make_unique<CreateTableMigration<AccountRow>>()
        );
        migration.addMigration(
            std::make_unique<CreateTableMigration<CashAccountRow>>()
        );
        _migrations.push_back(std::move(migration));

        assert(_migrations.size() == toVersion);
    }

    /**
     * @brief apply all migration steps
     *
     * @param db
     */
    void Migrations::migrate(db::Database& db)
    {
        for (std::size_t i = _fromVersion; i < _toVersion; ++i)
            _migrations[i].migrate(db);
    }

}   // namespace app
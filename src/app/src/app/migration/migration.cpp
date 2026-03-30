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
        assert(fromVersion <= toVersion);
        assert(_migrations.empty());

        // add migrations
        _migrateV1();
        _migrateV2();

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

    /**
     * @brief Migrate from version 1
     *
     * @details This handles the migration from v0 to v1. It creates the initial
     * profile, account, and cash account tables.
     *
     */
    void Migrations::_migrateV1()
    {
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
    }

    /**
     * @brief Migrate from version 2
     *
     * @details This handles the migration from v1 to v2. It introduces a unique
     * constraint on the account name and account kind field combination.
     */
    void Migrations::_migrateV2()
    {
        Migration migration(1);

        // For introducing unique constraints of account name and account kind

        // 1. we want to find any clashes between account names and kinds and
        // append the id
        std::string sql = std::format(
            R"(
                UPDATE {0}
                SET {1} = {1} || ' (' || CAST({2} AS TEXT) || ')'
                WHERE {2} NOT IN (
                    SELECT MIN({2}) FROM {0} GROUP BY {1}, {3}
                );
            )",
            AccountRow::tableName,
            decltype(AccountRow::name)::getColumnName(),
            decltype(AccountRow::id)::getColumnName(),
            decltype(AccountRow::kind)::getColumnName()
        );

        migration.addMigration(
            std::make_unique<CustomMigration>(
                std::move(sql),
                MigrationType::UpdateTable
            )
        );

        // 2. we need to turn off foreign key constraints
        migration.addMigration(std::make_unique<ChangeForeignKeyPragma>(false));

        // 3. we can create the new account table with the constraints with a
        // temporary name
        const auto newName = std::format("{}_tmp", AccountRow::tableName);
        migration.addMigration(
            std::make_unique<CreateTableMigration<AccountRow>>(newName)
        );

        // 4. Next we can insert everything form old account to temp account
        migration.addMigration(
            std::make_unique<CopyTableMigration>(AccountRow::tableName, newName)
        );

        // 5. now we can drop the old account table
        migration.addMigration(
            std::make_unique<DropTableMigration>(AccountRow::tableName)
        );

        // 6. now we can alter the new account table
        migration.addMigration(
            std::make_unique<RenameTableMigration>(
                newName,
                AccountRow::tableName
            )
        );

        // 7. finally turn foreign key constraints back on
        migration.addMigration(std::make_unique<ChangeForeignKeyPragma>(true));

        _migrations.push_back(std::move(migration));
    }

}   // namespace app
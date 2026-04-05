#include "app/migration/migration.hpp"

#include <cassert>

#include "app/migration/single_migration.hpp"
#include "db/database.hpp"
#include "orm/type_traits.hpp"
#include "sql_models/account_row.hpp"
#include "sql_models/instrument_row.hpp"
#include "sql_models/profile_row.hpp"
#include "sql_models/transaction_entry_row.hpp"
#include "sql_models/transaction_row.hpp"

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
        _migrateV3();
        _migrateV4();

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
     * @brief Copy, drop and rename a table
     *
     * @tparam Model The model representing the table
     * @param migration The migration to add the steps to
     */
    template <orm::db_model Model>
    void Migrations::_copyDropRename(Migration& migration)
    {
        migration.addMigration(std::make_unique<ChangeForeignKeyPragma>(false));

        std::string newName = Model::tableName + "_tmp";
        migration.addMigration(
            std::make_unique<CreateTableMigration<Model>>(newName)
        );
        migration.addMigration(
            std::make_unique<CopyTableMigration>(Model::tableName, newName)
        );
        migration.addMigration(
            std::make_unique<DropTableMigration>(Model::tableName)
        );
        migration.addMigration(
            std::make_unique<RenameTableMigration>(newName, Model::tableName)
        );

        migration.addMigration(std::make_unique<ChangeForeignKeyPragma>(true));
    }

    /**
     * @brief Migrate to version 1
     *
     * @details This handles the migration from v0 to v1.
     * It creates the initial tables:
     *  - profile
     *  - account
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
        _migrations.push_back(std::move(migration));
    }

    /**
     * @brief Migrate to version 2
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

        _copyDropRename<AccountRow>(migration);

        _migrations.push_back(std::move(migration));
    }

    /**
     * @brief Migrate to version 3
     *
     * @details This handles the migration from v2 to v3.
     * Creating tables:
     *  - instrument
     *  - transaction
     *  - transaction_entry
     */
    void Migrations::_migrateV3()
    {
        Migration migration(2);

        // Creating new tables: instrument, transaction, transaction_entry
        migration.addMigration(
            std::make_unique<CreateTableMigration<InstrumentRow>>()
        );
        migration.addMigration(
            std::make_unique<CreateTableMigration<TransactionRow>>()
        );
        migration.addMigration(
            std::make_unique<CreateTableMigration<TransactionEntryRow>>()
        );

        _migrations.push_back(std::move(migration));
    }

    /**
     * @brief Migrate to version 4
     *
     * @details This handles the migration from v3 to v4.
     */
    void Migrations::_migrateV4()
    {
        Migration migration(3);

        // Apply new constraints to account table...
        // here the unique constraint on (name, kind) gets extended to
        // (profileId, name, kind) which makes the migration straight forward as
        // the constraints only loosen up so we can just copy, drop, and rename
        _copyDropRename<AccountRow>(migration);

        _migrations.push_back(std::move(migration));
    }

}   // namespace app
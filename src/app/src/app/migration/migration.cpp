#include "app/migration/migration.hpp"

#include <cassert>
#include <memory>
#include <string>

#include "app/migration/multi_migration.hpp"
#include "app/migration/single_migration.hpp"
#include "config/finance.hpp"
#include "db/database.hpp"
#include "orm/constraints.hpp"
#include "sql_models/account_row.hpp"
#include "sql_models/instrument_row.hpp"
#include "sql_models/profile_row.hpp"
#include "sql_models/stock_row.hpp"
#include "sql_models/trade_leg_row.hpp"
#include "sql_models/transaction_entry_row.hpp"
#include "sql_models/transaction_row.hpp"
#include "utils/version.hpp"

namespace app
{

    /**
     * @brief Construct a new Migration object
     *
     * @param fromVersion The version the migration is being applied from
     * @param version The release version this migration is targeting
     */
    Migration::Migration(std::size_t fromVersion, utils::SemVer version)
        : _fromVersion(fromVersion), _version(version)
    {
    }

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
        _migrations.push_back(
            std::make_unique<MultiMigration>(std::move(migration))
        );
    }

    /**
     * @brief add a multi migration to the migration container
     *
     * @param migration
     */
    void Migration::addMigration(std::unique_ptr<MultiMigration> migration)
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
        _migrate_0_0_3();
        _migrate_0_1_0();

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
     * @brief Migrate from version 0.0.3
     */
    void Migrations::_migrate_0_0_3()
    {
        _lastReleaseVersion = utils::SemVer(0, 0, 3);

        _migrateV1();
        _migrateV2();
        _migrateV3();
        _migrateV4();
        _migrateV5();
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
        Migration migration(0, _lastReleaseVersion);
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
        Migration migration(1, _lastReleaseVersion);

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
            AccountRow::nameField::name,
            AccountRow::idField::name,
            AccountRow::kindField::name
        );

        migration.addMigration(
            std::make_unique<CustomMigration>(
                std::move(sql),
                MigrationType::UpdateTable
            )
        );

        migration.addMigration(
            std::make_unique<CopyDropRenameMigration<AccountRow>>()
        );

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
        Migration migration(2, _lastReleaseVersion);

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
        Migration migration(3, _lastReleaseVersion);

        // Apply new constraints to account table...
        // here the unique constraint on (name, kind) gets extended to
        // (profileId, name, kind) which makes the migration straight forward as
        // the constraints only loosen up so we can just copy, drop, and rename
        migration.addMigration(
            std::make_unique<CopyDropRenameMigration<AccountRow>>()
        );

        _migrations.push_back(std::move(migration));
    }

    /**
     * @brief Migrate to version 5
     *
     * @details This handles the migration from v4 to v5.
     */
    void Migrations::_migrateV5()
    {
        Migration migration(4, _lastReleaseVersion);

        // Add currency field to instrument table
        // ATTENTION: could be
        // problematic when migrating existing data as the currency could be out
        // of line, but as before this migration it was not possible to add
        // instruments to the database via the app this should be safe unless
        // someone decided to mess around with the db manually!!!
        using Field =
            InstrumentRow::Field<"currency", Currency, orm::not_null_t>;

        migration.addMigration(
            std::make_unique<AddColumnMigration<Field>>(Field{Currency::USD})
        );

        _migrations.push_back(std::move(migration));
    }

    /**
     * @brief Migrate from version 0.1.0
     *
     */
    void Migrations::_migrate_0_1_0()
    {
        _lastReleaseVersion = utils::SemVer(0, 1, 0);

        _migrateV6();
        _migrateV7();
        _migrateV8();
    }

    /**
     * @brief Migrate to version 6
     *
     * @details This handles the migration from v5 to v6. It drops and
     * recreates the transaction, transaction_entry, and instrument tables to
     * add new fields and constraints, and creates a new trade_leg table for
     * representing trade transactions in a more structured way.
     */
    void Migrations::_migrateV6()
    {
        constexpr std::size_t currentVersion = 5;
        Migration             migration(currentVersion, _lastReleaseVersion);

        migration.addMigration(
            std::make_unique<
                DropAndRecreateTableMigration<TransactionEntryRow>>()
        );

        migration.addMigration(
            std::make_unique<DropAndRecreateTableMigration<InstrumentRow>>()
        );

        migration.addMigration(
            std::make_unique<DropAndRecreateTableMigration<TransactionRow>>()
        );

        migration.addMigration(
            std::make_unique<CreateTableMigration<TradeLegRow>>()
        );

        _migrations.push_back(std::move(migration));
    }

    /**
     * @brief Migrate to version 7
     *
     * @details This handles the migration from v6 to v7. It creates a new
     * stock table for representing stock instruments, which has a one-to-one
     * relationship with the instrument table, allowing for more specific fields
     * related to stocks (e.g., ticker symbol) while still maintaining a common
     * base for all instruments.
     */
    void Migrations::_migrateV7()
    {
        constexpr std::size_t currentVersion = 6;
        Migration             migration(currentVersion, _lastReleaseVersion);

        migration.addMigration(
            std::make_unique<CreateTableMigration<StockRow>>()
        );

        _migrations.push_back(std::move(migration));
    }

    /**
     * @brief Migrate to version 8
     *
     * @details This handles the migration from v7 to v8. It adds a currency
     * field to the stock table for representing the currency in which the stock
     * is traded, and removes the currency field from the instrument table as it
     * is not relevant for all types of instruments and can lead to confusion.
     */
    void Migrations::_migrateV8()
    {
        constexpr std::size_t currentVersion = 7;
        Migration             migration(currentVersion, _lastReleaseVersion);

        // Add currency field to stock table
        // ATTENTION: could be
        // problematic when migrating existing data as the currency could be out
        // of line, but as before this migration it was not possible to add
        // stocks to the database via the app this should be safe unless
        // someone decided to mess around with the db manually!!!
        migration.addMigration(
            std::make_unique<AddColumnMigration<StockRow::currencyField>>(
                StockRow::currencyField{Currency::USD}
            )
        );

        migration.addMigration(
            std::make_unique<DropColumnMigration<InstrumentRow>>("currency")
        );

        _migrations.push_back(std::move(migration));
    }

}   // namespace app
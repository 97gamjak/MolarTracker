#include "app/migration/single_migration.hpp"

namespace app
{

    /**
     * @brief Construct a new Single Migration:: Single Migration object
     *
     * @param type
     */
    SingleMigration::SingleMigration(MigrationType type) : _type(type) {}

    void SingleMigration::setSQLStatements(
        const std::vector<std::string>& sqlStatements
    )
    {
        _sqlStatements = sqlStatements;
    }

    CustomMigration::CustomMigration(std::string sql)
        : SingleMigration(MigrationType::Custom), _sql(std::move(sql))
    {
    }

    CustomMigration::CustomMigration(std::string sql, MigrationType type)
        : SingleMigration(type), _sql(std::move(sql))
    {
    }

    void CustomMigration::applyMigration(db::Database& db)
    {
        // Apply the custom migration using the SQL statement
        db.execute(_sql);

        setSQLStatements(std::vector<std::string>{_sql});
    }

    ChangeForeignKeyPragma::ChangeForeignKeyPragma(bool enable)
        : SingleMigration(MigrationType::ChangeForeignKey), _enable(enable)
    {
    }

    void ChangeForeignKeyPragma::applyMigration(db::Database& db)
    {
        // Apply the change foreign key pragma migration
        std::string sql =
            std::format("PRAGMA foreign_keys = {}", _enable ? "ON" : "OFF");

        db.execute(sql);

        setSQLStatements(std::vector<std::string>{sql});
    }

    DropTableMigration::DropTableMigration(std::string tableName)
        : SingleMigration(MigrationType::DropTable),
          _tableName(std::move(tableName))
    {
    }

    void DropTableMigration::applyMigration(db::Database& db)
    {
        // Apply the drop table migration
        std::string sql = std::format("DROP TABLE {}", _tableName);

        db.execute(sql);

        setSQLStatements(std::vector<std::string>{sql});
    }

    CopyTableMigration::CopyTableMigration(
        std::string sourceTable,
        std::string destinationTable
    )
        : SingleMigration(MigrationType::CopyTable),
          _sourceTable(std::move(sourceTable)),
          _destinationTable(std::move(destinationTable))
    {
    }

    void CopyTableMigration::applyMigration(db::Database& db)
    {
        // Apply the copy table migration
        std::string sql = std::format(
            "INSERT INTO {} SELECT * FROM {}",
            _destinationTable,
            _sourceTable
        );

        db.execute(sql);

        setSQLStatements(std::vector<std::string>{sql});
    }

    RenameTableMigration::RenameTableMigration(
        std::string oldName,
        std::string newName
    )
        : SingleMigration(MigrationType::RenameTable),
          _oldName(std::move(oldName)),
          _newName(std::move(newName))
    {
    }

    void RenameTableMigration::applyMigration(db::Database& db)
    {
        // Apply the rename table migration
        std::string sql =
            std::format("ALTER TABLE {} RENAME TO {}", _oldName, _newName);

        db.execute(sql);

        setSQLStatements(std::vector<std::string>{sql});
    }

}   // namespace app

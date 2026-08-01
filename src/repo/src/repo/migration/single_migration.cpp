#include "single_migration.hpp"

#include <format>

namespace repo
{

    /**
     * @brief Construct a new Single Migration:: Single Migration object
     *
     * @param type
     */
    SingleMigration::SingleMigration(MigrationType type) : _type(type) {}

    /**
     * @brief Set the SQL statements for this migration
     *
     * @param sqlStatements
     */
    void SingleMigration::setSQLStatements(
        const std::vector<std::string>& sqlStatements
    )
    {
        _sqlStatements = sqlStatements;
    }

    /**
     * @brief Set the SQL statements for this migration
     *
     * @param sqlStatement
     */
    void SingleMigration::setSQLStatements(const std::string& sqlStatement)
    {
        _sqlStatements = std::vector<std::string>{sqlStatement};
    }

    /**
     * @brief Construct a new Custom Migration object
     *
     * @param sql
     */
    CustomMigration::CustomMigration(std::string sql)
        : SingleMigration(MigrationType::Custom), _sql(std::move(sql))
    {
    }

    /**
     * @brief Construct a new Custom Migration object
     *
     * @param sql
     * @param type
     */
    CustomMigration::CustomMigration(std::string sql, MigrationType type)
        : SingleMigration(type), _sql(std::move(sql))
    {
    }

    /**
     * @brief Apply the custom migration
     *
     * @param db
     *
     * @return CrudResult<void>
     */
    CrudResult<void> CustomMigration::applyMigration(db::Database& db)
    {
        // Apply the custom migration using the SQL statement
        const auto result = db.execute(_sql);

        if (!result)
        {
            return FromError<DatabaseError, CrudError>::apply(
                result.error(),
                "Failed to apply custom migration with SQL: " + _sql
            );
        }

        setSQLStatements(std::vector<std::string>{_sql});

        return {};
    }

    /**
     * @brief Construct a new Change Foreign Key Pragma object
     *
     * @param enable
     */
    ChangeForeignKeyPragma::ChangeForeignKeyPragma(bool enable)
        : SingleMigration(MigrationType::ChangeForeignKey), _enable(enable)
    {
    }

    /**
     * @brief Apply the change foreign key pragma migration
     *
     * @param db
     *
     * @return CrudResult<void>
     */
    CrudResult<void> ChangeForeignKeyPragma::applyMigration(db::Database& db)
    {
        // Apply the change foreign key pragma migration
        std::string sql =
            std::format("PRAGMA foreign_keys = {}", _enable ? "ON" : "OFF");

        const auto result = db.execute(sql);

        if (!result)
        {
            return FromError<DatabaseError, CrudError>::apply(
                result.error(),
                "Failed to apply change foreign key pragma migration to '" +
                    std::string(_enable ? "ON" : "OFF") + "'"
            );
        }

        setSQLStatements(std::vector<std::string>{sql});

        return {};
    }

    /**
     * @brief Construct a new Drop Table Migration object
     *
     * @param tableName
     */
    DropTableMigration::DropTableMigration(std::string tableName)
        : SingleMigration(MigrationType::DropTable),
          _tableName(std::move(tableName))
    {
    }

    /**
     * @brief Apply the drop table migration
     *
     * @param db
     *
     * @return CrudResult<void>
     */
    CrudResult<void> DropTableMigration::applyMigration(db::Database& db)
    {
        // Apply the drop table migration
        std::string sql = std::format("DROP TABLE {}", _tableName);

        const auto result = db.execute(sql);

        if (!result)
        {
            return FromError<DatabaseError, CrudError>::apply(
                result.error(),
                "Failed to apply drop table migration for table: " + _tableName
            );
        }

        setSQLStatements(std::vector<std::string>{sql});

        return {};
    }

    /**
     * @brief Construct a new Copy Table Migration object
     *
     * @param sourceTable
     * @param destinationTable
     */
    CopyTableMigration::CopyTableMigration(
        std::string sourceTable,
        std::string destinationTable
    )
        : SingleMigration(MigrationType::CopyTable),
          _sourceTable(std::move(sourceTable)),
          _destinationTable(std::move(destinationTable))
    {
    }

    /**
     * @brief Apply the copy table migration
     *
     * @param db
     *
     * @return CrudResult<void>
     */
    CrudResult<void> CopyTableMigration::applyMigration(db::Database& db)
    {
        // Apply the copy table migration
        std::string sql = std::format(
            "INSERT INTO {} SELECT * FROM {}",
            _destinationTable,
            _sourceTable
        );

        const auto result = db.execute(sql);

        if (!result)
        {
            return FromError<DatabaseError, CrudError>::apply(
                result.error(),
                "Failed to apply copy table migration from '" + _sourceTable +
                    "' to '" + _destinationTable + "'"
            );
        }

        setSQLStatements(std::vector<std::string>{sql});

        return {};
    }

    /**
     * @brief Construct a new Rename Table Migration object
     *
     * @param oldName
     * @param newName
     */
    RenameTableMigration::RenameTableMigration(
        std::string oldName,
        std::string newName
    )
        : SingleMigration(MigrationType::RenameTable),
          _oldName(std::move(oldName)),
          _newName(std::move(newName))
    {
    }

    /**
     * @brief Apply the rename table migration
     *
     * @param db
     *
     * @return CrudResult<void>
     */
    CrudResult<void> RenameTableMigration::applyMigration(db::Database& db)
    {
        // Apply the rename table migration
        std::string sql =
            std::format("ALTER TABLE {} RENAME TO {}", _oldName, _newName);

        const auto result = db.execute(sql);

        if (!result)
        {
            return FromError<DatabaseError, CrudError>::apply(
                result.error(),
                "Failed to apply rename table migration from '" + _oldName +
                    "' to '" + _newName + "'"
            );
        }

        setSQLStatements(std::vector<std::string>{sql});

        return {};
    }

}   // namespace repo

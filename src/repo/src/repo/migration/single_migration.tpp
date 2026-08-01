#ifndef __REPO__SRC__REPO__MIGRATION__SINGLE_MIGRATION_TPP__
#define __REPO__SRC__REPO__MIGRATION__SINGLE_MIGRATION_TPP__

#include "orm/crud.hpp"
#include "orm/type_traits.hpp"
#include "single_migration.hpp"

namespace repo
{

    /**
     * @brief construct a CreateTableMigration object
     *
     */
    template <orm::db_model Model>
    CreateTableMigration<Model>::CreateTableMigration()
        : SingleMigration(MigrationType::AddTable), _tableName(Model::tableName)
    {
    }

    /**
     * @brief Construct a new Create Table Migration object
     *
     * @param tableName
     */
    template <orm::db_model Model>
    CreateTableMigration<Model>::CreateTableMigration(std::string tableName)
        : SingleMigration(MigrationType::AddTable),
          _tableName(std::move(tableName))
    {
    }

    /**
     * @brief apply migration to create a table from Model
     *
     * @tparam Model
     * @param db
     *
     * @return CrudResult<void>
     */
    template <orm::db_model Model>
    CrudResult<void> CreateTableMigration<Model>::applyMigration(
        db::Database& db
    )
    {
        orm::Crud  crud;
        const auto result = crud.createTable<Model>(db, _tableName);

        if (!result)
            return result.error();

        setSQLStatements(crud.getExecutedSQL());
        return {};
    }

    /**
     * @brief Construct a new Add Column Migration< Field>:: Add Column
     * Migration object
     *
     * @tparam Field
     * @param defaultValue
     */
    template <typename Field>
    AddColumnMigration<Field>::AddColumnMigration(Field defaultValue)
        : SingleMigration(MigrationType::AddColumn), _defaultValue(defaultValue)
    {
    }

    /**
     * @brief Apply the migration to add a column to the database
     *
     * @param db The database to apply the migration to
     *
     * @return CrudResult<void> An empty expected on success, or an error on
     * failure
     */
    template <typename Field>
    CrudResult<void> AddColumnMigration<Field>::applyMigration(db::Database& db)
    {
        orm::Crud crud;

        const auto result = crud.addColumn(db, _defaultValue);
        if (!result)
            return result.error();

        setSQLStatements(crud.getExecutedSQL());
        return {};
    }

    /**
     * @brief Construct a new Drop Column Migration< Model>:: Drop Column
     * Migration object
     *
     * @tparam Model
     * @param columnName
     */
    template <typename Model>
    DropColumnMigration<Model>::DropColumnMigration(std::string columnName)
        : SingleMigration(MigrationType::DropColumn),
          _columnName(std::move(columnName))
    {
    }

    /**
     * @brief Apply the migration to drop a column from the database
     *
     * @tparam Model
     * @param db The database to apply the migration to
     *
     * @return CrudResult<void> An empty expected on success, or an error on
     * failure
     */
    template <typename Model>
    CrudResult<void> DropColumnMigration<Model>::applyMigration(
        db::Database& db
    )
    {
        orm::Crud crud;

        const auto result = crud.dropColumn<Model>(db, _columnName);
        if (!result)
            return result.error();

        setSQLStatements(crud.getExecutedSQL());
        return {};
    }

}   // namespace repo

#endif   // __REPO__SRC__REPO__MIGRATION__SINGLE_MIGRATION_TPP__

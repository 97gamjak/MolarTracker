#ifndef __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_TPP__
#define __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_TPP__

#include "app/migration/single_migration.hpp"
#include "db/statement.hpp"
#include "orm/constraints.hpp"
#include "orm/crud.hpp"
#include "orm/type_traits.hpp"

namespace app
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
     */
    template <orm::db_model Model>
    void CreateTableMigration<Model>::applyMigration(db::Database& db)
    {
        orm::Crud crud;
        crud.createTable<Model>(db, _tableName);

        setSQLStatements(crud.getExecutedSQL());
    }

    template <typename Field>
    InsertDefaultValuesMigration<Field>::InsertDefaultValuesMigration(
        Field defaultValue
    )
        : SingleMigration(MigrationType::Custom), _defaultValue(defaultValue)
    {
    }

    template <typename Field>
    void InsertDefaultValuesMigration<Field>::applyMigration(db::Database& db)
    {
        orm::Crud crud;

        const auto result = crud.update(db, _defaultValue);

        setSQLStatements(crud.getExecutedSQL());

        if (!result)
        {
            // TODO(97gamjak): introduce specific exception
            throw std::runtime_error("Failed to insert default values");
        }
    }

    template <typename Field>
    AddColumnMigration<Field>::AddColumnMigration(Field defaultValue)
        : SingleMigration(MigrationType::AddColumn), _defaultValue(defaultValue)
    {
    }

    template <typename Field>
    void AddColumnMigration<Field>::applyMigration(db::Database& db)
    {
        orm::Crud crud;

        crud.addColumn(db, _defaultValue);

        setSQLStatements(crud.getExecutedSQL());
    }

}   // namespace app

#endif   // __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_TPP__

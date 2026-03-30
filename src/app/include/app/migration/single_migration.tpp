#ifndef __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_TPP__
#define __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_TPP__

#include "app/migration/single_migration.hpp"
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

}   // namespace app

#endif   // __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_TPP__

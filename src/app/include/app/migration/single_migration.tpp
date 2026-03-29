#ifndef __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_TPP__
#define __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_TPP__

#include "app/migration/single_migration.hpp"
#include "orm/crud.hpp"

namespace app
{

    // TODO(97gamjak): use db_model concept
    template <typename Model>
    CreateTableMigration<Model>::CreateTableMigration()
        : SingleMigration(MigrationType::AddTable)
    {
    }

    template <typename Model>
    void CreateTableMigration<Model>::applyMigration(db::Database& db)
    {
        orm::createTable<Model>(db);
    }

}   // namespace app

#endif   // __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_TPP__

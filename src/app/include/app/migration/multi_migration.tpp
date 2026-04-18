#ifndef __APP__INCLUDE__APP__MIGRATION__MULTI_MIGRATION_TPP__
#define __APP__INCLUDE__APP__MIGRATION__MULTI_MIGRATION_TPP__

#include "app/migration/multi_migration.hpp"
#include "orm/type_traits.hpp"

namespace app
{

    template <orm::db_model Model>
    CopyDropRenameMigration<Model>::CopyDropRenameMigration()
    {
        addMigration(std::make_unique<ChangeForeignKeyPragma>(false));

        const auto  tableName = std::string(Model::tableName);
        std::string newName   = Model::tableName + "_tmp";

        addMigration(std::make_unique<CreateTableMigration<Model>>(newName));
        addMigration(std::make_unique<CopyTableMigration>(tableName, newName));
        addMigration(std::make_unique<DropTableMigration>(tableName));
        addMigration(
            std::make_unique<RenameTableMigration>(newName, tableName)
        );

        addMigration(std::make_unique<ChangeForeignKeyPragma>(true));
    }

}   // namespace app

#endif   // __APP__INCLUDE__APP__MIGRATION__MULTI_MIGRATION_TPP__

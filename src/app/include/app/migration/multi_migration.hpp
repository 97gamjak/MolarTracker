#ifndef __APP__INCLUDE__APP__MIGRATION__MULTI_MIGRATION_HPP__
#define __APP__INCLUDE__APP__MIGRATION__MULTI_MIGRATION_HPP__

#include <vector>

#include "app/migration/single_migration.hpp"
#include "orm/type_traits.hpp"

namespace app
{

    /**
     * @brief Migration to apply multiple single migrations
     *
     */
    class MultiMigration
    {
       private:
        /// the list of single migrations to apply
        std::vector<std::unique_ptr<SingleMigration>> _migrations;

       public:
        explicit MultiMigration() = default;
        explicit MultiMigration(std::unique_ptr<SingleMigration> migration);

        void addMigration(std::unique_ptr<SingleMigration> migration);

        void applyMigration(db::Database& db) const;
    };

    /**
     * @brief Migration to copy, drop, and rename a table
     *
     */
    template <orm::db_model Model>
    class CopyDropRenameMigration : public MultiMigration
    {
       public:
        CopyDropRenameMigration();
    };

    /**
     * @brief Migration to drop and recreate a table
     *
     */
    template <orm::db_model Model>
    class DropAndRecreateTableMigration : public MultiMigration
    {
       public:
        DropAndRecreateTableMigration();
    };

}   // namespace app

#ifndef __APP__INCLUDE__APP__MIGRATION__MULTI_MIGRATION_TPP__
#include "multi_migration.tpp"
#endif

#endif   // __APP__INCLUDE__APP__MIGRATION__MULTI_MIGRATION_HPP__

#ifndef __APP__INCLUDE__APP__MIGRATION__MIGRATION_HPP__
#define __APP__INCLUDE__APP__MIGRATION__MIGRATION_HPP__

#include <cstddef>
#include <memory>
#include <mstd/enum.hpp>
#include <vector>

#include "app/migration/single_migration.hpp"

namespace db
{
    class Database;
}   // namespace db

namespace app
{
    /**
     * @brief Represents one version of a database migration
     */
    class Migration
    {
       private:
        /// The version the migration is being applied from
        std::size_t _fromVersion;

        /// collection of single migrations
        std::vector<std::unique_ptr<SingleMigration>> _migrations;

       public:
        explicit Migration(std::size_t fromVersion);

        void migrate(db::Database& db) const;
        void addMigration(std::unique_ptr<SingleMigration> migration);
    };

    /**
     * @brief Represents a collection of database migrations
     */
    class Migrations
    {
       private:
        /// The version the migration is being applied from
        std::size_t _fromVersion;
        /// The version the migration is being applied to
        std::size_t _toVersion;

        /// The list of individual migrations
        std::vector<Migration> _migrations;

       public:
        Migrations(std::size_t fromVersion, std::size_t toVersion);

        void migrate(db::Database& db);

       private:
        void _migrateV1();
        void _migrateV2();
        void _migrateV3();
        void _migrateV4();

        template <orm::db_model Model>
        void _copyDropRename(Migration& migration);
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__MIGRATION__MIGRATION_HPP__
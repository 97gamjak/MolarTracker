#ifndef __REPO__SRC__REPO__MIGRATION__MIGRATION_HPP__
#define __REPO__SRC__REPO__MIGRATION__MIGRATION_HPP__

#include <cstddef>
#include <memory>
#include <mstd/enum.hpp>
#include <vector>

#include "common/version.hpp"
#include "multi_migration.hpp"

namespace db
{
    class Database;
}   // namespace db

namespace repo
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
        std::vector<std::unique_ptr<MultiMigration>> _migrations;

        /// The release version this migration is targeting
        common::SemVer _version;

       public:
        explicit Migration(
            std::size_t           fromVersion,
            const common::SemVer& version
        );

        void migrate(db::Database& db) const;
        void addMigration(std::unique_ptr<SingleMigration> migration);
        void addMigration(std::unique_ptr<MultiMigration> migration);
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

        /// The last release version
        common::SemVer _lastReleaseVersion =
            common::SemVer::getInvalidVersion();

       public:
        Migrations(std::size_t fromVersion, std::size_t toVersion);

        void migrate(db::Database& db);

       private:
        void _migrate_0_0_3();
        void _migrateV1();
        void _migrateV2();
        void _migrateV3();
        void _migrateV4();
        void _migrateV5();
        void _migrate_0_1_0();
        void _migrateV6();
        void _migrateV7();
        void _migrateV8();
        void _migrateV9();
        void _migrateV10();
        void _migrate_0_2_3();
        void _migrateV11();
        void _migrateV12();
        void _migrateV13();
        void _migrateV14();
        void _migrateV15();
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__MIGRATION__MIGRATION_HPP__
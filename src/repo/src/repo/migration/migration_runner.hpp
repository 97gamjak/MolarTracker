#ifndef __REPO__SRC__REPO__MIGRATION__MIGRATION_RUNNER_HPP__
#define __REPO__SRC__REPO__MIGRATION__MIGRATION_RUNNER_HPP__

#include "migration.hpp"

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace repo
{
    /**
     * @brief Class responsible for running database migrations
     */
    class MigrationRunner
    {
       private:
        /// current db version
        constexpr static std::size_t DB_VERSION = 14;

        /// The migration states for the application
        Migrations _migrations;

       public:
        explicit MigrationRunner(db::Database& db);

       private:
        void migrate(db::Database& db);
    };
}   // namespace repo

#endif   // __REPO__SRC__REPO__MIGRATION__MIGRATION_RUNNER_HPP__

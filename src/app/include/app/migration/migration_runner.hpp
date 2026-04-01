#ifndef __APP__INCLUDE__APP__MIGRATION__MIGRATION_RUNNER_HPP__
#define __APP__INCLUDE__APP__MIGRATION__MIGRATION_RUNNER_HPP__

#include "app/migration/migration.hpp"

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace app
{
    /**
     * @brief Class responsible for running database migrations
     */
    class MigrationRunner
    {
       private:
        /// current db version
        constexpr static std::size_t DB_VERSION = 3;

        /// The migration states for the application
        Migrations _migrations;

       public:
        explicit MigrationRunner(db::Database& db);

       private:
        void migrate(db::Database& db);
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__MIGRATION__MIGRATION_RUNNER_HPP__

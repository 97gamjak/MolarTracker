#ifndef __APP__INCLUDE__APP__MIGRATION__MIGRATION_RUNNER_HPP__
#define __APP__INCLUDE__APP__MIGRATION__MIGRATION_RUNNER_HPP__

#include "app/migration/migration_state.hpp"

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace app
{
    class MigrationRunner
    {
       private:
        MigrationStates _migrationStates;

       public:
        explicit MigrationRunner(db::Database& db);
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__MIGRATION__MIGRATION_RUNNER_HPP__

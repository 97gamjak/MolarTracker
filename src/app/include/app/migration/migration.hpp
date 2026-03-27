#ifndef __APP__INCLUDE__APP__MIGRATION__MIGRATION_HPP__
#define __APP__INCLUDE__APP__MIGRATION__MIGRATION_HPP__

#include <functional>
#include <vector>

#include "app/migration/migration_state.hpp"

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace app
{
    class MigrationStates;

    /**
     * @brief Class for handling database migrations
     *
     */
    class Migration
    {
       private:
        static constexpr int MigrationVersion = 1;

        using MigrationFunc = std::function<void(db::Database& db)>;

        static constexpr std::vector<MigrationFunc> _migrations{};

       public:
        Migration();

        static MigrationStates migrate(db::Database& db);
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__MIGRATION__MIGRATION_HPP__
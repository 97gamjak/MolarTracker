#ifndef __APP__INCLUDE__APP__MIGRATION__MIGRATION_STATE_HPP__
#define __APP__INCLUDE__APP__MIGRATION__MIGRATION_STATE_HPP__

#include <cstddef>
#include <vector>

namespace app
{
    class MigrationState
    {
       private:
        std::size_t _fromVersion;

       public:
        explicit MigrationState(std::size_t fromVersion);
    };

    class MigrationStates
    {
       private:
        std::size_t _fromVersion;
        std::size_t _toVersion;

        std::vector<MigrationState> _states;

       public:
        MigrationStates(std::size_t fromVersion, std::size_t toVersion);

        static MigrationStates createInvalidState();

        void addMigration(const MigrationState& state);
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__MIGRATION__MIGRATION_STATE_HPP__
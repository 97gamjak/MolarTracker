#include "app/migration/migration_state.hpp"

namespace app
{
    MigrationState::MigrationState(std::size_t fromVersion)
        : _fromVersion(fromVersion)
    {
    }

    MigrationStates::MigrationStates(
        std::size_t fromVersion,
        std::size_t toVersion
    )
        : _fromVersion(fromVersion), _toVersion(toVersion)
    {
    }

    MigrationStates MigrationStates::createInvalidState()
    {
        return MigrationStates{0, 0};
    }

    void MigrationStates::addMigration(const MigrationState& state)
    {
        _states.push_back(state);
    }

}   // namespace app
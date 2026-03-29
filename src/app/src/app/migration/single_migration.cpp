#include "app/migration/single_migration.hpp"

namespace app
{

    /**
     * @brief Construct a new Single Migration:: Single Migration object
     *
     * @param type
     */
    SingleMigration::SingleMigration(MigrationType type) : _type(type) {}

    void SingleMigration::setSQLStatements(
        const std::vector<std::string>& sqlStatements
    )
    {
        _sqlStatements = sqlStatements;
    }

}   // namespace app

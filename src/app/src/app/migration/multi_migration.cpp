#include "app/migration/multi_migration.hpp"

#include <memory>

namespace app
{
    /**
     * @brief Construct a new Multi Migration object
     *
     * @param migration
     */
    MultiMigration::MultiMigration(std::unique_ptr<SingleMigration> migration)
    {
        addMigration(std::move(migration));
    }

    /**
     * @brief Add a migration to the multi-migration
     *
     * @param migration
     */
    void MultiMigration::addMigration(
        std::unique_ptr<SingleMigration> migration
    )
    {
        _migrations.push_back(std::move(migration));
    }

    /**
     * @brief Apply the migration to the database
     *
     * @param db The database to apply the migration to
     */
    void MultiMigration::applyMigration(db::Database& db) const
    {
        for (const auto& migration : _migrations)
        {
            migration->applyMigration(db);
        }
    }
}   // namespace app
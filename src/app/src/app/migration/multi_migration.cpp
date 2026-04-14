#include "app/migration/multi_migration.hpp"

#include <memory>

namespace app
{
    MultiMigration::MultiMigration(std::unique_ptr<SingleMigration> migration)
    {
        addMigration(std::move(migration));
    }

    void MultiMigration::addMigration(
        std::unique_ptr<SingleMigration> migration
    )
    {
        _migrations.push_back(std::move(migration));
    }

    void MultiMigration::applyMigration(db::Database& db)
    {
        for (const auto& migration : _migrations)
        {
            migration->applyMigration(db);
        }
    }
}   // namespace app
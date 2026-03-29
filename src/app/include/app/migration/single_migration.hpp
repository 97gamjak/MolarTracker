#ifndef __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_HPP__
#define __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <string>

namespace db
{
    class Database;
}   // namespace db

namespace app
{
#define MIGRATION_TYPE_LIST(X) X(AddTable)

    MSTD_ENUM(MigrationType, std::uint8_t, MIGRATION_TYPE_LIST);

    class SingleMigration
    {
       private:
        MigrationType _type;
        std::string   _description;

       public:
        explicit SingleMigration(MigrationType type);
        virtual ~SingleMigration() = default;

        virtual void applyMigration(db::Database& db) = 0;
    };

    // TODO(97gamjak): use db_model concept
    template <typename Model>
    class CreateTableMigration : public SingleMigration
    {
       public:
        explicit CreateTableMigration();

        void applyMigration(db::Database& db) override;
    };
}   // namespace app

#ifndef __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_TPP__
#include "app/migration/single_migration.tpp"
#endif

#endif   // __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_HPP__
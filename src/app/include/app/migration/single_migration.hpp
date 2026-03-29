#ifndef __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_HPP__
#define __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <string>
#include <vector>

#include "orm/type_traits.hpp"

namespace db
{
    class Database;
}   // namespace db

namespace app
{
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define MIGRATION_TYPE_LIST(X) X(AddTable)

    MSTD_ENUM(MigrationType, std::uint8_t, MIGRATION_TYPE_LIST);

    /**
     * @brief Base class for a single migration, this is used to represent a
     * single migration that can be applied to the database, such as creating a
     * table for a specific model
     *
     */
    class SingleMigration
    {
       private:
        /// the type of the migration
        MigrationType _type;

        /// the description of the migration
        std::string _description;

        /// the SQL statements executed by this migration
        std::vector<std::string> _sqlStatements;

       public:
        explicit SingleMigration(MigrationType type);
        virtual ~SingleMigration() = default;

        void setSQLStatements(const std::vector<std::string>& sqlStatements);

        /**
         * @brief pure virtual function to apply a single migration that needs
         * to be implemented by all concrete single migrations
         *
         * @param db
         */
        virtual void applyMigration(db::Database& db) = 0;
    };

    /**
     * @brief Migration to create a table for a specific model
     *
     */
    template <orm::db_model Model>
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
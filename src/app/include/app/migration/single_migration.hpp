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
#define MIGRATION_TYPE_LIST(X) \
    X(AddTable)                \
    X(UpdateTable)             \
    X(ChangeForeignKey)        \
    X(Custom)                  \
    X(DropTable)               \
    X(CopyTable)               \
    X(RenameTable)

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
       private:
        std::string _tableName;

       public:
        explicit CreateTableMigration();
        explicit CreateTableMigration(std::string tableName);

        void applyMigration(db::Database& db) override;
    };

    class CustomMigration : public SingleMigration
    {
       private:
        std::string _sql;

       public:
        explicit CustomMigration(std::string sql);
        explicit CustomMigration(std::string sql, MigrationType type);

        void applyMigration(db::Database& db) override;
    };

    /**
     * @brief Migration to change the foreign key constraints
     *
     */
    class ChangeForeignKeyPragma : public SingleMigration
    {
       private:
        bool _enable;

       public:
        explicit ChangeForeignKeyPragma(bool enable);

        void applyMigration(db::Database& db) override;
    };

    class DropTableMigration : public SingleMigration
    {
       private:
        std::string _tableName;

       public:
        explicit DropTableMigration(std::string tableName);

        void applyMigration(db::Database& db) override;
    };

    class CopyTableMigration : public SingleMigration
    {
       private:
        std::string _sourceTable;
        std::string _destinationTable;

       public:
        CopyTableMigration(
            std::string sourceTable,
            std::string destinationTable
        );

        void applyMigration(db::Database& db) override;
    };

    class RenameTableMigration : public SingleMigration
    {
       private:
        std::string _oldName;
        std::string _newName;

       public:
        RenameTableMigration(std::string oldName, std::string newName);

        void applyMigration(db::Database& db) override;
    };

}   // namespace app

#ifndef __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_TPP__
#include "app/migration/single_migration.tpp"
#endif

#endif   // __APP__INCLUDE__APP__MIGRATION__SINGLE_MIGRATION_HPP__
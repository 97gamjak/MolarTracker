#include "app/migration/migration_runner.hpp"

#include "app/migration/migration.hpp"
#include "db/database.hpp"
#include "db/transaction.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("App.Migration");

namespace app
{
    /**
     * @brief Construct a new Migration Exception:: Migration Exception object
     *
     * @param message
     * @param dbPath
     */
    MigrationException::MigrationException(
        const std::string& message,
        std::string        dbPath
    )
        : MolarTrackerException(message), _dbPath(std::move(dbPath))
    {
    }

    /**
     * @brief Get the error message
     *
     * @return const char* The error message
     */
    const char* MigrationException::what() const noexcept
    {
        const std::string prefix =
            "MigrationException for database (" + _dbPath + "): ";
        static std::string fullMessage;

        fullMessage = prefix + MolarTrackerException::what();
        return fullMessage.c_str();
    }

    /**
     * @brief Construct a new MigrationRunner object
     *
     * @param db The database to migrate
     */
    MigrationRunner::MigrationRunner(db::Database& db)
        : _migrations(0, DB_VERSION)
    {
        try
        {
            migrate(db);
        }
        catch (const std::exception& e)
        {
            throw MigrationException(e.what(), db.getDBPath());
        }
    }

    /**
     * @brief determine last db version and apply all migrations needed
     *
     * @param db
     */
    void MigrationRunner::migrate(db::Database& db)
    {
        const auto dbVersion =
            static_cast<size_t>(db.queryInt("PRAGMA user_version"));

        LOG_INFO(
            "Applying migrations: " + std::to_string(dbVersion) + " -> " +
            std::to_string(DB_VERSION)
        );

        db.makeBackup();

        _migrations = Migrations{dbVersion, DB_VERSION};

        db::Transaction transaction{db};

        _migrations.migrate(db);

        db.execute("PRAGMA user_version = " + std::to_string(DB_VERSION));

        transaction.commit();
    }

}   // namespace app

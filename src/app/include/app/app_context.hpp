#ifndef __APP__INCLUDE__APP__APP_CONTEXT_HPP__
#define __APP__INCLUDE__APP__APP_CONTEXT_HPP__

#include "app/migration/migration_runner.hpp"
#include "app/repo_container.hpp"
#include "app/service_container.hpp"
#include "app/store_container.hpp"
#include "db/database.hpp"
#include "settings/settings.hpp"

namespace app
{

    /**
     * @brief Application context holding settings, database, repositories,
     * services, and store
     *
     */
    class AppContext
    {
       private:
        /// The settings object for the application, which can be used to
        /// subscribe to settings changes
        settings::Settings& _settings;

        /// The database instance for the application
        db::Database _database;

        MigrationRunner _migrationRunner;

        /// The repository container for the application
        RepoContainer _repos;

        /// The service container for the application
        ServiceContainer _services;

        /// The store container for the application, this is where the global
        /// state of the application is stored and can be accessed and modified
        /// by the controllers and other parts of the application
        StoreContainer _store;

       public:
        explicit AppContext(settings::Settings& settings);
        ~AppContext() = default;

        StoreContainer&       getStore();
        const StoreContainer& getStore() const;

        settings::Settings&       getSettings();
        const settings::Settings& getSettings() const;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__APP_CONTEXT_HPP__
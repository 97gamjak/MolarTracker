#ifndef __APP__INCLUDE__APP__APP_CONTEXT_HPP__
#define __APP__INCLUDE__APP__APP_CONTEXT_HPP__

#include <memory>

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace settings
{
    class Settings;   // Forward declaration
}   // namespace settings

namespace app
{
    class RepoContainer;      // Forward declaration
    class ServiceContainer;   // Forward declaration
    class StoreContainer;     // Forward declaration
    class MigrationRunner;    // Forward declaration
}   // namespace app

namespace app
{

    /**
     * @brief Application context holding settings,
     * services, and store
     *
     */
    class AppContext
    {
       private:
        /// The settings object for the application, which can be used to
        /// subscribe to settings changes
        settings::Settings& _settings;

        /// The service container for the application
        std::unique_ptr<ServiceContainer> _services;

        /// The store container for the application, this is where the global
        /// state of the application is stored and can be accessed and modified
        /// by the controllers and other parts of the application
        std::unique_ptr<StoreContainer> _store;

       public:
        explicit AppContext(settings::Settings& settings);
        ~AppContext();

        [[nodiscard]]
        StoreContainer& getStore();
        [[nodiscard]]
        const StoreContainer& getStore() const;

        [[nodiscard]]
        settings::Settings& getSettings();
        [[nodiscard]]
        const settings::Settings& getSettings() const;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__APP_CONTEXT_HPP__
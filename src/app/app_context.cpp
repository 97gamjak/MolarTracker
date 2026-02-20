#include "app_context.hpp"

#include "config/constants.hpp"
#include "db/database.hpp"

namespace app
{

    /**
     * @brief Construct a new App Context:: App Context object
     *
     * @param settings The settings object to use for the application context
     */
    AppContext::AppContext(settings::Settings& settings)
        : _settings{settings},
          _database{Constants::getInstance().getDatabasePath()},
          _repos{_database},
          _services{_repos},
          _store{_services}
    {
    }

    /**
     * @brief Get the store container
     *
     * @return StoreContainer&
     */
    StoreContainer& AppContext::getStore() { return _store; }

    /**
     * @brief Get the store container (const version)
     *
     * @return const StoreContainer&
     */
    const StoreContainer& AppContext::getStore() const { return _store; }

    /**
     * @brief Get the settings object
     *
     * @return settings::Settings&
     */
    settings::Settings& AppContext::getSettings() { return _settings; }

    /**
     * @brief Get the settings object (const version)
     *
     * @return const settings::Settings&
     */
    const settings::Settings& AppContext::getSettings() const
    {
        return _settings;
    }
}   // namespace app

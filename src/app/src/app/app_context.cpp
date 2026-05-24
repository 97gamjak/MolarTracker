#include "app/app_context.hpp"

#include <memory>

#include "app/store_container.hpp"

namespace app
{

    /**
     * @brief Construct a new App Context:: App Context object
     *
     * @param settings The settings object to use for the application context
     */
    AppContext::AppContext(settings::Settings& settings)
        : _settings{settings}, _store{std::make_unique<StoreContainer>()}
    {
    }

    /**
     * @brief Destroy the App Context:: App Context object
     *
     */
    AppContext::~AppContext() = default;

    /**
     * @brief Get the store container
     *
     * @return StoreContainer&
     */
    StoreContainer& AppContext::getStore() { return *_store; }

    /**
     * @brief Get the store container (const version)
     *
     * @return const StoreContainer&
     */
    const StoreContainer& AppContext::getStore() const { return *_store; }

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

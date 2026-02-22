#include "store_container.hpp"

#include "service_container.hpp"

#define __LOG_CATEGORY__ LogCategory::app_storeContainer
#include "logging/log_macros.hpp"

namespace app
{

    /**
     * @brief Construct a new Store Container object
     *
     * @param services
     */
    StoreContainer::StoreContainer(ServiceContainer& services)
        : _profileStore{services.getProfileService()}
    {
        _allStores.push_back(&_profileStore);
    }

    /**
     * @brief Save all temporary changes to the database
     *
     */
    void StoreContainer::commit()
    {
        LOG_INFO("Saving all temporary changes to database");

        for (auto* store : _allStores)
            store->commit();
    }

    /**
     * @brief Subscribe to changes in the dirty state of any store, the
     * provided callback function will be called whenever the dirty state of any
     * store changes, the user pointer can be used to pass additional data to
     * the callback function, the returned Connection object can be used to
     * unsubscribe from changes
     *
     * @param func The callback function to call when the dirty state of any
     * store changes, it should have the signature void(void* user, bool
     * isDirty)
     * @param user A user-defined pointer that will be passed to the callback
     * function when it is called, this can be used to provide additional
     * context for the callback function
     * @return std::vector<Connection> A vector of Connection objects
     * representing the subscriptions, these can be used to unsubscribe from
     * changes by calling disconnect() on them or by letting them go out of
     * scope
     */
    std::vector<Connection> StoreContainer::subscribeToDirty(
        OnDirtyChanged::func func,
        void*                user
    )
    {
        std::vector<Connection> connections;
        connections.reserve(_allStores.size());

        for (auto* store : _allStores)
            connections.push_back(store->subscribeToDirty(func, user));

        return connections;
    }

    /**
     * @brief Get the ProfileStore
     *
     * @return ProfileStore&
     */
    ProfileStore& StoreContainer::getProfileStore() { return _profileStore; }

    /**
     * @brief Get the ProfileStore (const version)
     *
     * @return const ProfileStore&
     */
    const ProfileStore& StoreContainer::getProfileStore() const
    {
        return _profileStore;
    }

}   // namespace app
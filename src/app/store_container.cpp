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
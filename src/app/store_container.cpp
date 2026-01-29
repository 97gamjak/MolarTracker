#include "store_container.hpp"

#include "service_container.hpp"

#define __LOG_CATEGORY__ LogCategory::app_storeContainer
#include "logging/log_macros.hpp"

namespace app
{

    StoreContainer::StoreContainer(ServiceContainer& services)
        : _profileStore{services.profileService()}
    {
    }

    void StoreContainer::commit()
    {
        LOG_INFO("Saving all temporary changes to database");

        for (auto* store : _allStores)
            store->commit();
    }

}   // namespace app
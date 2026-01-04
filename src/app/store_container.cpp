#include "app/store_container.hpp"

#include "app/service_container.hpp"

namespace app
{

    StoreContainer::StoreContainer(ServiceContainer& services)
        : _profileStore{services.profileService()}
    {
    }

}   // namespace app
#ifndef __APP__STORE__STORE_CONTAINER_HPP__
#define __APP__STORE__STORE_CONTAINER_HPP__

#include "store/profile_store.hpp"

namespace app
{

    class ServiceContainer;   // Forward declaration

    class StoreContainer
    {
       private:
        ProfileStore _profileStore;

       public:
        StoreContainer(ServiceContainer& services);

        // clang-format off
        ProfileStore&       getProfileStore() noexcept { return _profileStore; }
        const ProfileStore& getProfileStore() const noexcept { return _profileStore; }
        // clang-format on
    };

}   // namespace app

#endif   // __APP__STORE__STORE_CONTAINER_HPP__
#ifndef __APP__STORE__STORE_CONTAINER_HPP__
#define __APP__STORE__STORE_CONTAINER_HPP__

#include "store/profile_store.hpp"

namespace app
{

    class ServiceContainer;   // Forward declaration

    /**
     * @brief Container for all stores
     *
     */
    class StoreContainer
    {
       private:
        ProfileStore         _profileStore;
        std::vector<IStore*> _allStores{
            &_profileStore,
        };

       public:
        StoreContainer(ServiceContainer& services);

        void commit();

        // clang-format off
        ProfileStore&       getProfileStore() { return _profileStore; }
        const ProfileStore& getProfileStore() const { return _profileStore; }
        // clang-format on
    };

}   // namespace app

#endif   // __APP__STORE__STORE_CONTAINER_HPP__
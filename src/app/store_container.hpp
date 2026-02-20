#ifndef __APP__STORE_CONTAINER_HPP__
#define __APP__STORE_CONTAINER_HPP__

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
        /// The Profile store
        ProfileStore _profileStore;

        /// list of all stores TODO: maybe introduce a std::tie
        std::vector<IStore*> _allStores{
            &_profileStore,
        };

       public:
        explicit StoreContainer(ServiceContainer& services);

        void commit();

        ProfileStore&       getProfileStore();
        const ProfileStore& getProfileStore() const;
    };

}   // namespace app

#endif   // __APP__STORE_CONTAINER_HPP__
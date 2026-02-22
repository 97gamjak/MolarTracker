#ifndef __APP__STORE_CONTAINER_HPP__
#define __APP__STORE_CONTAINER_HPP__

#include "config/signal_tags.hpp"
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

        /// list of all stores
        std::vector<IStore*> _allStores;

       public:
        explicit StoreContainer(ServiceContainer& services);

        void commit();
        void clearPotentiallyDirty();

        std::vector<Connection> subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        );

        ProfileStore&       getProfileStore();
        const ProfileStore& getProfileStore() const;
    };

}   // namespace app

#endif   // __APP__STORE_CONTAINER_HPP__
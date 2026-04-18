#ifndef __APP__INCLUDE__APP__STORE_CONTAINER_HPP__
#define __APP__INCLUDE__APP__STORE_CONTAINER_HPP__

#include "app/store/account_store.hpp"
#include "app/store/profile/profile_store.hpp"
#include "app/store/transaction_store.hpp"
#include "config/signal_tags.hpp"
#include "connections/connection.hpp"

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

        /// The Account store
        AccountStore _accountStore;

        /// The Transaction store
        TransactionStore _transactionStore;

        /// list of all stores
        std::vector<IStore*> _allStores;

        /// list of connections for all stores
        std::vector<Connection> _connections;

       public:
        explicit StoreContainer(ServiceContainer& services);

        void commit();
        void clearPotentiallyDirty();

        std::vector<Connection> subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        );

        [[nodiscard]] ProfileStore&       getProfileStore();
        [[nodiscard]] const ProfileStore& getProfileStore() const;

        [[nodiscard]] AccountStore&       getAccountStore();
        [[nodiscard]] const AccountStore& getAccountStore() const;

        [[nodiscard]] TransactionStore&       getTransactionStore();
        [[nodiscard]] const TransactionStore& getTransactionStore() const;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE_CONTAINER_HPP__
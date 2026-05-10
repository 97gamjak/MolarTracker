#ifndef __APP__INCLUDE__APP__STORE_CONTAINER_HPP__
#define __APP__INCLUDE__APP__STORE_CONTAINER_HPP__

#include <memory>
#include <vector>

#include "config/id_types.hpp"
#include "config/signal_tags.hpp"

class Connections;   // Forward declaration

namespace app
{

    class ServiceContainer;   // Forward declaration
    class ProfileStore;       // Forward declaration
    class AccountStore;       // Forward declaration
    class StockStore;         // Forward declaration
    class PositionStore;      // Forward declaration
    class TransactionStore;   // Forward declaration
    class IStore;             // Forward declaration

    /**
     * @brief Container for all stores
     *
     */
    class StoreContainer
    {
       private:
        /// The instrument ID sequence
        InstrumentIdSeq _instrumentIdSeq;
        /// The Profile store
        std::unique_ptr<ProfileStore> _profileStore;
        /// The Account store
        std::unique_ptr<AccountStore> _accountStore;
        /// The stock store
        std::unique_ptr<StockStore> _stockStore;
        /// The Position store
        std::unique_ptr<PositionStore> _positionStore;
        /// The Transaction store
        std::unique_ptr<TransactionStore> _transactionStore;

        /// list of all stores
        std::vector<IStore*> _allStores;

        /// list of connections for all stores
        std::unique_ptr<Connections> _connections;

       public:
        explicit StoreContainer(ServiceContainer& services);

        ~StoreContainer();

        void commit();
        void clearPotentiallyDirty();
        bool isDirty() const;

        Connections subscribeToDirty(
            const OnDirtyChanged::func& func,
            void*                       user
        );

        [[nodiscard]] ProfileStore&       getProfileStore();
        [[nodiscard]] const ProfileStore& getProfileStore() const;

        [[nodiscard]] AccountStore&       getAccountStore();
        [[nodiscard]] const AccountStore& getAccountStore() const;

        [[nodiscard]] TransactionStore&       getTransactionStore();
        [[nodiscard]] const TransactionStore& getTransactionStore() const;

        [[nodiscard]] StockStore&       getStockStore();
        [[nodiscard]] const StockStore& getStockStore() const;

        [[nodiscard]] PositionStore&       getPositionStore();
        [[nodiscard]] const PositionStore& getPositionStore() const;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE_CONTAINER_HPP__
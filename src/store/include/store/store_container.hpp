#ifndef __STORE__INCLUDE__STORE__STORE_CONTAINER_HPP__
#define __STORE__INCLUDE__STORE__STORE_CONTAINER_HPP__

#include <memory>
#include <vector>

#include "config/id_types.hpp"
#include "config/signal_tags.hpp"

class Connections;   // Forward declaration

namespace service
{
    class ServiceContainer;   // Forward declaration
}   // namespace service

namespace store
{

    class IProfileStore;       // Forward declaration
    class IAccountStore;       // Forward declaration
    class IStockStore;         // Forward declaration
    class PositionStore;       // Forward declaration
    class ITransactionStore;   // Forward declaration
    class IStore;              // Forward declaration

    /**
     * @brief Container for all stores
     *
     */
    class StoreContainer
    {
       private:
        /// The service container
        std::unique_ptr<service::ServiceContainer> _serviceContainer;
        /// The instrument ID sequence
        InstrumentIdSeq _instrumentIdSeq;
        /// The Profile store
        std::shared_ptr<IProfileStore> _profileStore;
        /// The Account store
        std::shared_ptr<IAccountStore> _accountStore;
        /// The stock store
        std::shared_ptr<IStockStore> _stockStore;
        /// The Position store
        std::unique_ptr<PositionStore> _positionStore;
        /// The Transaction store
        std::shared_ptr<ITransactionStore> _transactionStore;

        /// list of all stores
        std::vector<IStore*> _allStores;

        /// list of connections for all stores
        std::unique_ptr<Connections> _connections;

       public:
        explicit StoreContainer();

        ~StoreContainer();

        void               commit();
        void               clearPotentiallyDirty();
        [[nodiscard]] bool isDirty() const;

        Connections subscribeToDirty(
            const OnDirtyChanged::func& func,
            void*                       user
        );

        [[nodiscard]] std::shared_ptr<IProfileStore>&       getProfileStore();
        [[nodiscard]] const std::shared_ptr<IProfileStore>& getProfileStore(
        ) const;

        [[nodiscard]] std::shared_ptr<IAccountStore>&       getAccountStore();
        [[nodiscard]] const std::shared_ptr<IAccountStore>& getAccountStore(
        ) const;

        [[nodiscard]] std::shared_ptr<ITransactionStore>& getTransactionStore();
        [[nodiscard]] const std::shared_ptr<ITransactionStore>& getTransactionStore(
        ) const;

        [[nodiscard]] std::shared_ptr<IStockStore>&       getStockStore();
        [[nodiscard]] const std::shared_ptr<IStockStore>& getStockStore() const;

        [[nodiscard]] PositionStore&       getPositionStore();
        [[nodiscard]] const PositionStore& getPositionStore() const;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__STORE_CONTAINER_HPP__
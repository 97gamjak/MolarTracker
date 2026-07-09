#ifndef __STORE__INCLUDE__STORE__STORE_CONTAINER_HPP__
#define __STORE__INCLUDE__STORE__STORE_CONTAINER_HPP__

#include <memory>

#include "config/id_types.hpp"
#include "config/signal_tags.hpp"

class Connections;   // Forward declaration

namespace service
{
    class ServiceContainer;   // Forward declaration
}   // namespace service

namespace store
{

    class IProfileStore;             // Forward declaration
    class IAccountStore;             // Forward declaration
    class IAccountStoreReader;       // Forward declaration
    class IStockStore;               // Forward declaration
    class IStockStoreReader;         // Forward declaration
    class IPositionStore;            // Forward declaration
    class IPositionStoreReader;      // Forward declaration
    class ITransactionStore;         // Forward declaration
    class ITransactionStoreReader;   // Forward declaration
    class IStore;                    // Forward declaration
    class IOptionStore;              // Forward declaration

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

        struct StoreImpl;
        /// The implementation of the store container, this is used to hide the
        /// details of the store implementations and allow for a clean interface
        /// for the store container, while still providing the necessary
        /// functionality to manage and access the various stores within the
        /// application.
        std::unique_ptr<StoreImpl> _stores;

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

        //
        // NOTE: no const ref returns here because of the shared_ptrs to be able
        // to return readers and writers from the same stored shared_ptr
        //

        [[nodiscard]] std::shared_ptr<IProfileStore> getProfileStore() const;

        [[nodiscard]]
        std::shared_ptr<IAccountStore> getAccountStore() const;
        [[nodiscard]]
        std::shared_ptr<IAccountStoreReader> getAccountStoreReader() const;

        [[nodiscard]]
        std::shared_ptr<ITransactionStore> getTransactionStore() const;
        [[nodiscard]]
        std::shared_ptr<ITransactionStoreReader> getTransactionStoreReader(
        ) const;

        [[nodiscard]]
        std::shared_ptr<IStockStore> getStockStore() const;
        [[nodiscard]]
        std::shared_ptr<IStockStoreReader> getStockStoreReader() const;

        [[nodiscard]] std::shared_ptr<IOptionStore> getOptionStore() const;

        [[nodiscard]]
        std::shared_ptr<IPositionStore> getPositionStore() const;
        [[nodiscard]]
        std::shared_ptr<IPositionStoreReader> getPositionStoreReader() const;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__STORE_CONTAINER_HPP__
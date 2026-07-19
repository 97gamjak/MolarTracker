#ifndef __STORE__INCLUDE__STORE__STORE_CONTAINER_HPP__
#define __STORE__INCLUDE__STORE__STORE_CONTAINER_HPP__

#include <filesystem>
#include <memory>

#include "config/id_types.hpp"
#include "config/signal_tags.hpp"

class Connections;   // Forward declaration

namespace service
{
    class ServiceContainer;   // Forward declaration
}   // namespace service

namespace settings
{
    class BackupSettings;   // Forward declaration
}   // namespace settings

namespace store
{

    class IProfileStore;       // Forward declaration
    class IAccountStore;       // Forward declaration
    class IStockStore;         // Forward declaration
    class IPositionStore;      // Forward declaration
    class ITransactionStore;   // Forward declaration
    class IStore;              // Forward declaration
    class IOptionStore;        // Forward declaration

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
        explicit StoreContainer(const settings::BackupSettings& backupSettings);
        ~StoreContainer();

        void               commit();
        void               clearPotentiallyDirty();
        [[nodiscard]] bool isDirty() const;

        void restoreFromBackup(const std::filesystem::path& backupFile);

        Connections subscribeToDirty(
            const OnDirtyChanged::func& func,
            void*                       user
        );

        [[nodiscard]] std::shared_ptr<IProfileStore>  getProfileStore() const;
        [[nodiscard]] std::shared_ptr<IAccountStore>  getAccountStore() const;
        [[nodiscard]] std::shared_ptr<IStockStore>    getStockStore() const;
        [[nodiscard]] std::shared_ptr<IOptionStore>   getOptionStore() const;
        [[nodiscard]] std::shared_ptr<IPositionStore> getPositionStore() const;
        [[nodiscard]] std::shared_ptr<ITransactionStore> getTransactionStore(
        ) const;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__STORE_CONTAINER_HPP__
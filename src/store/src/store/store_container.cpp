#include "store/store_container.hpp"

#include <algorithm>
#include <memory>

#include "connections/connection.hpp"
#include "logging/log_macros.hpp"
#include "service/service_container.hpp"
#include "store/account/account_store.hpp"
#include "store/i_profile_store.hpp"
#include "store/option_store.hpp"
#include "store/position_store.hpp"
#include "store/profile/profile_store.hpp"
#include "store/stock_store.hpp"
#include "store/transaction_store.hpp"

REGISTER_LOG_CATEGORY("Store.StoreContainer");

namespace store
{

    /**
     * @brief Implementation of the StoreContainer, this class is responsible
     * for managing the various stores in the application, providing a
     * centralized location for accessing and managing the different stores, and
     * handling the interactions between them, such as committing changes,
     * clearing dirty states, and checking if any store is dirty, allowing for a
     * structured and organized way to manage the data stores within the
     * application.
     *
     */
    struct StoreContainer::StoreImpl
    {
        /// list of all stores
        std::vector<IStore*> allStores;

        /// The Profile store
        std::shared_ptr<ProfileStore> profileStore;

        /// The Account store
        std::shared_ptr<AccountStore> accountStore;

        /// The stock store
        std::shared_ptr<StockStore> stockStore;

        /// The option store
        std::shared_ptr<OptionStore> optionStore;

        /// The Position store
        std::shared_ptr<PositionStore> positionStore;

        /// The Transaction store
        std::shared_ptr<TransactionStore> transactionStore;

        StoreImpl(
            service::ServiceContainer& serviceContainer,
            InstrumentIdSeq&           instrumentIdSeq
        );
    };

    /**
     * @brief Construct a new Store Container:: Store Impl:: Store Impl object
     *
     * @param serviceContainer
     * @param instrumentIdSeq
     */
    StoreContainer::StoreImpl::StoreImpl(
        service::ServiceContainer& serviceContainer,
        InstrumentIdSeq&           instrumentIdSeq
    )
        : profileStore(
              std::make_shared<ProfileStore>(serviceContainer.getProfileService(
              ))
          ),
          accountStore(
              std::make_shared<AccountStore>(serviceContainer.getAccountService(
              ))
          ),
          stockStore(
              std::make_shared<StockStore>(
                  serviceContainer.getInstrumentService(),
                  instrumentIdSeq
              )
          ),
          optionStore(
              std::make_shared<OptionStore>(
                  serviceContainer.getInstrumentService(),
                  instrumentIdSeq
              )
          ),
          positionStore(
              std::make_shared<PositionStore>(
                  serviceContainer.getPositionService(),
                  accountStore->getAccountSession()
              )
          ),
          transactionStore(
              std::make_shared<TransactionStore>(
                  serviceContainer.getTransactionService(),
                  accountStore->getAccountSession()
              )
          )
    {
        allStores.push_back(profileStore.get());
        allStores.push_back(accountStore.get());
        allStores.push_back(stockStore.get());
        allStores.push_back(optionStore.get());
        allStores.push_back(positionStore.get());
        allStores.push_back(transactionStore.get());
    }

    /**
     * @brief Construct a new Store Container object
     *
     */
    StoreContainer::StoreContainer()
        : _serviceContainer{std::make_unique<service::ServiceContainer>()},
          _stores{
              std::make_unique<StoreImpl>(*_serviceContainer, _instrumentIdSeq)
          },
          _connections{std::make_unique<Connections>()}
    {
        _connections->add(_stores->profileStore->subscribeToProfileChange(
            [&](const std::optional<ProfileId>& profileId)
            { _stores->accountStore->updateActiveProfile(profileId); },
            &_stores->accountStore
        ));
    }

    /**
     * @brief Destroy the Store Container:: Store Container object
     *
     */
    StoreContainer::~StoreContainer() = default;

    /**
     * @brief Save all temporary changes to the database
     *
     */
    void StoreContainer::commit()
    {
        LOG_INFO("Saving all temporary changes to database");

        _stores->profileStore->commit();
        // here the id of the active profile store was already updated via
        // the observer in account store
        _stores->accountStore->commit();

        _stores->positionStore->commit();

        _stores->stockStore->commit();

        auto instrumentIdRemap = _stores->stockStore->getInstrumentIdMap();

        _stores->optionStore->commit(instrumentIdRemap);

        const auto& accountIdRemap  = _stores->accountStore->getIdRemap();
        const auto& positionIdRemap = _stores->positionStore->getIdRemap();

        if (!instrumentIdRemap.combine(_stores->optionStore->getInstrumentIdMap(
            )))
        {
            throw std::runtime_error(
                "Failed to combine instrument ID remaps from stock and option "
                "stores"
            );
        }

        _stores->transactionStore
            ->commit(accountIdRemap, instrumentIdRemap, positionIdRemap);

        for (auto* store : _stores->allStores)
        {
            if (store == nullptr)
                throw std::runtime_error("Store is null");

            store->clearIdRemap();
        }
    }

    /**
     * @brief Clear the potentially dirty state of all stores
     *
     */
    void StoreContainer::clearPotentiallyDirty()
    {
        for (auto* store : _stores->allStores)
        {
            if (store == nullptr)
                throw std::runtime_error("Store is null");

            store->clearPotentiallyDirty();
        }
    }

    /**
     * @brief Check if any store is dirty
     *
     * @return true if any store is dirty, false otherwise
     */
    bool StoreContainer::isDirty() const
    {
        return std::ranges::any_of(
            _stores->allStores,
            [](const auto* store)
            {
                if (store == nullptr)
                    throw std::runtime_error("Store is null");

                return store->isDirty();
            }
        );
    }

    /**
     * @brief Subscribe to changes in the dirty state of any store, the
     * provided callback function will be called whenever the dirty state of
     * any store changes, the user pointer can be used to pass additional
     * data to the callback function, the returned Connection object can be
     * used to unsubscribe from changes
     *
     * @param func The callback function to call when the dirty state of any
     * store changes, it should have the signature void(void* user, bool
     * isDirty)
     * @param user A user-defined pointer that will be passed to the
     * callback function when it is called, this can be used to provide
     * additional context for the callback function
     * @return Connections A Connections object representing the
     * subscriptions, these can be used to unsubscribe from changes by
     * calling disconnect() on them or by letting them go out of scope
     */
    Connections StoreContainer::subscribeToDirty(
        const OnDirtyChanged::func& func,
        void*                       user
    )
    {
        Connections connections;

        for (auto* store : _stores->allStores)
        {
            if (store == nullptr)
                throw std::runtime_error("Store is null");

            connections.add(store->subscribeToDirty(func, user));
        }

        return connections;
    }

    /**
     * @brief Get the ProfileStore (const version)
     *
     * @return std::shared_ptr<IProfileStore>
     */
    std::shared_ptr<IProfileStore> StoreContainer::getProfileStore() const
    {
        return _stores->profileStore;
    }

    /**
     * @brief Get the AccountStore (const version)
     *
     * @return std::shared_ptr<IAccountStore>
     */
    std::shared_ptr<IAccountStore> StoreContainer::getAccountStore() const
    {
        return _stores->accountStore;
    }

    /**
     * @brief Get the TransactionStore (const version)
     *
     * @return std::shared_ptr<ITransactionStore>
     */
    std::shared_ptr<ITransactionStore> StoreContainer::getTransactionStore(
    ) const
    {
        return _stores->transactionStore;
    }

    /**
     * @brief Get the StockStore (const version)
     *
     * @return std::shared_ptr<IStockStore>
     */
    std::shared_ptr<IStockStore> StoreContainer::getStockStore() const
    {
        return _stores->stockStore;
    }

    /**
     * @brief Get the StockStoreReader (const version)
     *
     * @return std::shared_ptr<IStockStoreReader>
     */
    std::shared_ptr<IStockStoreReader> StoreContainer::getStockStoreReader(
    ) const
    {
        return _stores->stockStore;
    }

    /**
     * @brief Get the OptionStore (const version)
     *
     * @return std::shared_ptr<IOptionStore>
     */
    std::shared_ptr<IOptionStore> StoreContainer::getOptionStore() const
    {
        return _stores->optionStore;
    }

    /**
     * @brief Get the PositionStore (const version)
     *
     * @return std::shared_ptr<IPositionStore>
     */
    std::shared_ptr<IPositionStore> StoreContainer::getPositionStore() const
    {
        return _stores->positionStore;
    }

}   // namespace store
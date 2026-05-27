#include "store/store_container.hpp"

#include <algorithm>
#include <memory>
#include <optional>
#include <string>

#include "connections/connection.hpp"
#include "logging/log_macros.hpp"
#include "service/service_container.hpp"
#include "store/account/account_store.hpp"
#include "store/i_profile_store.hpp"
#include "store/position_store.hpp"
#include "store/profile/profile_store.hpp"
#include "store/stock_store.hpp"
#include "store/transaction_store.hpp"

REGISTER_LOG_CATEGORY("Store.StoreContainer");

namespace store
{

    /**
     * @brief Construct a new Store Container object
     *
     */
    StoreContainer::StoreContainer()
        : _serviceContainer{std::make_unique<service::ServiceContainer>()},
          _profileStore{std::make_shared<ProfileStore>(
              _serviceContainer->getProfileService()
          )},
          _accountStore{std::make_shared<AccountStore>(
              _serviceContainer->getAccountService()
          )},
          _stockStore{std::make_unique<StockStore>(
              _serviceContainer->getInstrumentService(),
              _instrumentIdSeq
          )},
          _positionStore{std::make_unique<PositionStore>(
              _serviceContainer->getPositionService(),
              _accountStore->getAccountSession()
          )},
          _transactionStore{std::make_unique<TransactionStore>(
              _serviceContainer->getTransactionService(),
              *_stockStore,
              *_positionStore,
              _accountStore->getAccountSession()
          )},
          _connections{std::make_unique<Connections>()}
    {
        auto* profileStore = dynamic_cast<ProfileStore*>(_profileStore.get());
        auto* accountStore = dynamic_cast<AccountStore*>(_accountStore.get());

        if (profileStore == nullptr || !_profileStore ||
            accountStore == nullptr || !_accountStore || !_stockStore ||
            !_positionStore || !_transactionStore)
        {
            throw std::runtime_error("Failed to initialize store container");
        }

        _allStores.push_back(profileStore);
        _allStores.push_back(accountStore);
        _allStores.push_back(&*_transactionStore);
        _allStores.push_back(&*_stockStore);
        _allStores.push_back(&*_positionStore);

        _connections->add(_profileStore->subscribeToProfileChange(
            [&](const std::optional<ProfileId>& profileId)
            { _accountStore->updateActiveProfile(profileId); },
            &_accountStore
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

        _profileStore->commit();
        // here the id of the active profile store was already updated via
        // the observer in account store
        _accountStore->commit();

        _positionStore->commit();

        _stockStore->commit();

        _transactionStore->commit(_accountStore->getIdRemap());
    }

    /**
     * @brief Clear the potentially dirty state of all stores
     *
     */
    void StoreContainer::clearPotentiallyDirty()
    {
        for (auto* store : _allStores)
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
            _allStores,
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
     * @return Connections A Connections object representing the subscriptions,
     * these can be used to unsubscribe from changes by calling disconnect() on
     * them or by letting them go out of scope
     */
    Connections StoreContainer::subscribeToDirty(
        const OnDirtyChanged::func& func,
        void*                       user
    )
    {
        Connections connections;

        for (auto* store : _allStores)
        {
            if (store == nullptr)
                throw std::runtime_error("Store is null");

            connections.add(store->subscribeToDirty(func, user));
        }

        return connections;
    }

    /**
     * @brief Get the ProfileStore
     *
     * @return std::shared_ptr<IProfileStore>&
     */
    std::shared_ptr<IProfileStore>& StoreContainer::getProfileStore()
    {
        return _profileStore;
    }

    /**
     * @brief Get the ProfileStore (const version)
     *
     * @return const ProfileStore&
     */
    const std::shared_ptr<IProfileStore>& StoreContainer::getProfileStore(
    ) const
    {
        return _profileStore;
    }

    /**
     * @brief Get the AccountStore
     *
     * @return std::shared_ptr<IAccountStore>&
     */
    std::shared_ptr<IAccountStore>& StoreContainer::getAccountStore()
    {
        return _accountStore;
    }

    /**
     * @brief Get the AccountStore (const version)
     *
     * @return const std::shared_ptr<IAccountStore>&
     */
    const std::shared_ptr<IAccountStore>& StoreContainer::getAccountStore(
    ) const
    {
        return _accountStore;
    }

    /**
     * @brief Get the TransactionStore
     *
     * @return TransactionStore&
     */
    TransactionStore& StoreContainer::getTransactionStore()
    {
        return *_transactionStore;
    }

    /**
     * @brief Get the TransactionStore (const version)
     *
     * @return const TransactionStore&
     */
    const TransactionStore& StoreContainer::getTransactionStore() const
    {
        return *_transactionStore;
    }

    /**
     * @brief Get the StockStore
     *
     * @return StockStore&
     */
    StockStore& StoreContainer::getStockStore() { return *_stockStore; }

    /**
     * @brief Get the StockStore (const version)
     *
     * @return const StockStore&
     */
    const StockStore& StoreContainer::getStockStore() const
    {
        return *_stockStore;
    }

    /**
     * @brief Get the PositionStore
     *
     * @return PositionStore&
     */
    PositionStore& StoreContainer::getPositionStore()
    {
        return *_positionStore;
    }

    /**
     * @brief Get the PositionStore (const version)
     *
     * @return const PositionStore&
     */
    const PositionStore& StoreContainer::getPositionStore() const
    {
        return *_positionStore;
    }

}   // namespace store
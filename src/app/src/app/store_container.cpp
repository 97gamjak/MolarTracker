#include "app/store_container.hpp"

#include <algorithm>

#include "app/service_container.hpp"
#include "app/store/stock_store.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("App.Store.StoreContainer");

namespace app
{

    /**
     * @brief Construct a new Store Container object
     *
     * @param services
     */
    StoreContainer::StoreContainer(ServiceContainer& services)
        : _profileStore{services.getProfileService()},
          _accountStore{services.getAccountService()},
          _transactionStore{services.getTransactionService()},
          _stockStore{services.getInstrumentService()}
    {
        _allStores.push_back(&_profileStore);
        _allStores.push_back(&_accountStore);
        _allStores.push_back(&_transactionStore);
        _allStores.push_back(&_stockStore);

        auto connectProfileIdUpdate =
            [](void* user, const std::optional<ProfileId>& profileId)
        { static_cast<AccountStore*>(user)->updateActiveProfile(profileId); };

        _connections.push_back(_profileStore.subscribeToProfileChange(
            connectProfileIdUpdate,
            &_accountStore
        ));
    }

    /**
     * @brief Save all temporary changes to the database
     *
     */
    void StoreContainer::commit()
    {
        LOG_INFO("Saving all temporary changes to database");

        _profileStore.commit();
        // here the id of the active profile store was already updated via
        // the observer in account store
        _accountStore.commit();

        _transactionStore.commit(_accountStore.getChangedIds());
    }

    /**
     * @brief Clear the potentially dirty state of all stores
     *
     */
    void StoreContainer::clearPotentiallyDirty()
    {
        for (auto* store : _allStores)
            store->clearPotentiallyDirty();
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
            [](const auto* store) { return store->isDirty(); }
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
     * @return std::vector<Connection> A vector of Connection objects
     * representing the subscriptions, these can be used to unsubscribe from
     * changes by calling disconnect() on them or by letting them go out of
     * scope
     */
    std::vector<Connection> StoreContainer::subscribeToDirty(
        const OnDirtyChanged::func& func,
        void*                       user
    )
    {
        std::vector<Connection> connections;
        connections.reserve(_allStores.size());

        for (auto* store : _allStores)
            connections.push_back(store->subscribeToDirty(func, user));

        return connections;
    }

    /**
     * @brief Get the ProfileStore
     *
     * @return ProfileStore&
     */
    ProfileStore& StoreContainer::getProfileStore() { return _profileStore; }

    /**
     * @brief Get the ProfileStore (const version)
     *
     * @return const ProfileStore&
     */
    const ProfileStore& StoreContainer::getProfileStore() const
    {
        return _profileStore;
    }

    /**
     * @brief Get the AccountStore
     *
     * @return AccountStore&
     */
    AccountStore& StoreContainer::getAccountStore() { return _accountStore; }

    /**
     * @brief Get the AccountStore (const version)
     *
     * @return const AccountStore&
     */
    const AccountStore& StoreContainer::getAccountStore() const
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
        return _transactionStore;
    }

    /**
     * @brief Get the TransactionStore (const version)
     *
     * @return const TransactionStore&
     */
    const TransactionStore& StoreContainer::getTransactionStore() const
    {
        return _transactionStore;
    }

    /**
     * @brief Get the StockStore
     *
     * @return StockStore&
     */
    StockStore& StoreContainer::getStockStore() { return _stockStore; }

    /**
     * @brief Get the StockStore (const version)
     *
     * @return const StockStore&
     */
    const StockStore& StoreContainer::getStockStore() const
    {
        return _stockStore;
    }

}   // namespace app
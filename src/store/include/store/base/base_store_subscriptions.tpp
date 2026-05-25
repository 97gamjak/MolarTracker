#ifndef __STORE__INCLUDE__STORE__BASE__BASE_STORE_SUBSCRIPTIONS_TPP__
#define __STORE__INCLUDE__STORE__BASE__BASE_STORE_SUBSCRIPTIONS_TPP__

#include "base_store.hpp"
#include "config/signal_tags.hpp"

namespace store
{
    /**
     * @brief Subscribes a callback function to be called when the dirty state
     * of the store changes. The callback will receive a boolean indicating
     * whether the store is now dirty (true) or not (false).
     *
     * @tparam T
     * @tparam IdType
     * @param func
     * @param user
     * @return Connection
     */

    template <typename T, typename IdType>
    Connection BaseStore<T, IdType>::subscribeToDirty(
        OnDirtyChanged::func func,
        void*                user
    )
    {
        return this->template on<OnDirtyChanged>(func, user);
    }

    /**
     * @brief Subscribes a callback function to be called when a new entry is
     * added to the store.
     *
     * @tparam T
     * @tparam IdType
     * @param func
     * @param user
     * @return Connection
     */
    template <typename T, typename IdType>
    Connection BaseStore<T, IdType>::subscribeToIdRemap(
        OnIdRemap<IdType>::func func,
        void*                   user
    )
    {
        return this->template on<OnIdRemap<IdType>>(func, user);
    }

    /**
     * @brief Subscribes a callback function to be called when an entry is
     * removed from the store.
     *
     * @tparam T
     * @tparam IdType
     * @param func
     * @param user
     * @return Connection
     */
    template <typename T, typename IdType>
    Connection BaseStore<T, IdType>::subscribeToEntryRemoved(
        OnStoreItemRemoved<IdType>::func func,
        void*                            user
    )
    {
        return this->template on<OnStoreItemRemoved<IdType>>(func, user);
    }

    /**
     * @brief Subscribes a callback function to be called when an entry is
     * updated in the store.
     *
     * @tparam T
     * @tparam IdType
     * @param func
     * @param user
     * @return Connection
     */
    template <typename T, typename IdType>
    Connection BaseStore<T, IdType>::subscribeToEntryUpdated(
        OnStoreItemUpdated<T>::func func,
        void*                       user
    )
    {
        return this->template on<OnStoreItemUpdated<T>>(func, user);
    }

    /**
     * @brief Subscribes a callback function to be called when an entry is added
     * to the store.
     *
     * @tparam T
     * @tparam IdType
     * @param func
     * @param user
     * @return Connection
     */
    template <typename T, typename IdType>
    Connection BaseStore<T, IdType>::subscribeToEntryAdded(
        OnStoreItemAdded<T>::func func,
        void*                     user
    )
    {
        return this->template on<OnStoreItemAdded<T>>(func, user);
    }

    /**
     * @brief Subscribes a callback function to be called when an entry is added
     * to the store.
     *
     * @tparam T
     * @tparam IdType
     * @param func
     * @param user
     * @return Connection
     */
    template <typename T, typename IdType>
    Connection BaseStore<T, IdType>::subscribeToStoreChange(
        StoreChanged<IdType>::func func,
        void*                      user
    )
    {
        return this->template on<StoreChanged<IdType>>(func, user);
    }

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__BASE__BASE_STORE_SUBSCRIPTIONS_TPP__
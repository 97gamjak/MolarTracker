#ifndef __STORE__SRC__STORE__BASE__BASE_STORE_NOTIFICATIONS_TPP__
#define __STORE__SRC__STORE__BASE__BASE_STORE_NOTIFICATIONS_TPP__

#include "base_store.hpp"
#include "config/signal_tags.hpp"

namespace store
{
    /**
     * @brief Notifies subscribers of updated entries.
     *
     * @tparam T
     * @tparam IdType
     * @param value
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_notifyUpdated(const T& value)
    {
        this->template notify<OnStoreItemUpdated<T>>(value);
    }

    /**
     * @brief Notifies subscribers of added entries.
     *
     * @tparam T
     * @tparam IdType
     * @param value
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_notifyAdded(const T& value)
    {
        this->template notify<OnStoreItemAdded<T>>(value);
    }

    /**
     * @brief Notifies subscribers of removed entries.
     *
     * @tparam T
     * @tparam IdType
     * @param value
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_notifyRemoved(const IdType& id)
    {
        this->template notify<OnStoreItemRemoved<IdType>>(id);
    }

}   // namespace store

#endif   // __STORE__SRC__STORE__BASE__BASE_STORE_NOTIFICATIONS_TPP__
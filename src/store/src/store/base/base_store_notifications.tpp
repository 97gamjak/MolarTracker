#ifndef __STORE__SRC__STORE__BASE__BASE_STORE_NOTIFICATIONS_TPP__
#define __STORE__SRC__STORE__BASE__BASE_STORE_NOTIFICATIONS_TPP__

#include "base_store.hpp"

namespace store
{
    /**
     * @brief Notifies subscribers of updated entries.
     *
     * @tparam T
     * @tparam IdType
     * @param checkAlreadyNotified
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_notifyUpdated(bool checkAlreadyNotified)
    {
        this->template notify<OnStoreItemUpdated<T>>(_updated);
        _notifyStoreChanged(checkAlreadyNotified);
        _updated.clear();
    }

    /**
     * @brief Notifies subscribers of added entries.
     *
     * @tparam T
     * @tparam IdType
     * @param checkAlreadyNotified
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_notifyAdded(bool checkAlreadyNotified)
    {
        this->template notify<OnStoreItemAdded<T>>(_added);
        _notifyStoreChanged(checkAlreadyNotified);
        _added.clear();
    }

    /**
     * @brief Notifies subscribers of removed entries.
     *
     * @tparam T
     * @tparam IdType
     * @param checkAlreadyNotified
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_notifyRemoved(bool checkAlreadyNotified)
    {
        this->template notify<OnStoreItemRemoved<IdType>>(_removed);
        _notifyStoreChanged(checkAlreadyNotified);
        _removed.clear();
    }

    /**
     * @brief Notifies subscribers of store changes.
     *
     * @tparam T
     * @tparam IdType
     * @param checkAlreadyNotified
     */
    template <typename T, typename IdType>
    void BaseStore<T, IdType>::_notifyStoreChanged(bool checkAlreadyNotified)
    {
        if (checkAlreadyNotified)
        {
            if (!_alreadyNotified)
            {
                this->template notify<StoreChanged<IdType>>();
                _alreadyNotified = true;
            }
        }
        else
        {
            this->template notify<StoreChanged<IdType>>();
            // if we do not want to check it we even set it back
            _alreadyNotified = false;
        }
    }
}   // namespace store

#endif   // __STORE__SRC__STORE__BASE__BASE_STORE_NOTIFICATIONS_TPP__
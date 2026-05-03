#ifndef __APP__INCLUDE__APP__STORE__BASE__BASE_STORE_SUBSCRIPTIONS_TPP__
#define __APP__INCLUDE__APP__STORE__BASE__BASE_STORE_SUBSCRIPTIONS_TPP__

#include "base_store.hpp"

namespace app
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
    ) const
    {
        return this->template on<OnIdRemap<IdType>>(func, user);
    }

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__BASE__BASE_STORE_SUBSCRIPTIONS_TPP__
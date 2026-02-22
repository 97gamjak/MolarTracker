#include "i_store.hpp"

#include "config/signal_tags.hpp"

namespace app
{

    /**
     * @brief Mark the store as potentially dirty, this should be called after
     * making any changes to the store to indicate that there may be dirty data
     * that needs to be committed.
     */
    void IStore::_markPotentiallyDirty()
    {
        _isPotentiallyDirty = true;
        _notifySubscribers(true);
    }

    /**
     * @brief Mark the store as clean, this should be called after committing
     * changes to the store to indicate that there is no longer any dirty data
     * that needs to be committed.
     */
    void IStore::_clearPotentiallyDirty()
    {
        _isPotentiallyDirty = false;
        _notifySubscribers(false);
    }

    /**
     * @brief Subscribe to changes in the dirty state of the store, the
     * provided callback function will be called whenever the dirty state
     * changes, the user pointer can be used to pass additional data to the
     * callback function, the returned Connection object can be used to
     * unsubscribe from changes
     *
     * @param func The callback function to call when the dirty state of the
     * store changes, it should have the signature void(void* user, bool
     * isDirty)
     * @param user A user-defined pointer that will be passed to the callback
     * function when it is called, this can be used to provide additional
     * context for the callback function
     * @return Connection An object representing the subscription, this can be
     * used to unsubscribe from changes by calling disconnect() on it or by
     * letting it go out of scope
     */
    Connection IStore::subscribeToDirty(OnDirtyChanged::func func, void* user)
    {
        return Base::template on<OnDirtyChanged>(func, user);
    }

    /**
     * @brief Notify subscribers of a change in the dirty state of the store,
     * this should be called whenever the dirty state of the store changes to
     * notify any subscribers of the change, it takes a boolean indicating
     * whether the store is dirty (has unsaved changes) or not, and passes this
     * to the subscribers when notifying them of the change
     *
     * @param isDirty A boolean indicating whether the store is dirty (has
     * unsaved changes) or not, this will be passed to the subscribers when
     * notifying them of the change
     */
    void IStore::_notifySubscribers(const bool& isDirty)
    {
        Base::template _emit<OnDirtyChanged>(isDirty);
    }

}   // namespace app
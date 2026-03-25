#ifndef __APP__INCLUDE__APP__STORE__BASE__I_STORE_HPP__
#define __APP__INCLUDE__APP__STORE__BASE__I_STORE_HPP__

#include "config/signal_tags.hpp"
#include "connections/observable.hpp"

class Connection;   // Forward declaration

namespace app
{
    /**
     * @brief Interface for a generic store
     *
     */
    class IStore
    {
       public:
        virtual ~IStore() = default;

        /**
         * @brief Commit the changes to the store, this should be called
         * after making any changes to the store to ensure that the changes
         * are saved and can be accessed by other parts of the application.
         * The exact behavior of this method will depend on the
         * implementation of the store, but it may involve saving the
         * changes to a database, writing them to a file, or simply marking
         * them as committed in memory.
         *
         */
        virtual void commit() = 0;

        /**
         * @brief Check if the store has any dirty data that needs to be
         * committed
         *
         * @return true if the store has dirty data, false otherwise
         */
        [[nodiscard]] virtual bool isDirty() const = 0;

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
         * @param user A user-defined pointer that will be passed to the
         * callback function when it is called, this can be used to provide
         * additional context for the callback function
         * @return Connection An object representing the subscription, this can
         * be used to unsubscribe from changes by calling disconnect() on it or
         * by letting it go out of scope
         */
        virtual Connection subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        ) = 0;

        /**
         * @brief Mark the store as clean, this should be called after
         * committing changes to the store to indicate that there is no
         * longer any dirty data that needs to be committed.
         */
        virtual void clearPotentiallyDirty() = 0;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__BASE__I_STORE_HPP__
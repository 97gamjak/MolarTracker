#ifndef __APP__STORE__I_STORE_HPP__
#define __APP__STORE__I_STORE_HPP__

#include <cstdint>

#include "config/signal_tags.hpp"
#include "connections/observable.hpp"

class Connection;   // Forward declaration

namespace app
{
    /**
     * @brief Interface for a generic store
     *
     */
    class IStore : public Observable<OnDirtyChanged>
    {
       private:
        /// Type alias for the base class
        using Base = Observable<OnDirtyChanged>;

       private:
        /// Flag to indicate if the store has potentially dirty data that needs
        /// to be committed
        bool _isPotentiallyDirty = false;

       public:
        virtual ~IStore() = default;

        /**
         * @brief Commit the changes to the store, this should be called after
         * making any changes to the store to ensure that the changes are saved
         * and can be accessed by other parts of the application. The exact
         * behavior of this method will depend on the implementation of the
         * store, but it may involve saving the changes to a database, writing
         * them to a file, or simply marking them as committed in memory.
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

        Connection subscribeToDirty(OnDirtyChanged::func func, void* user);
        void       clearPotentiallyDirty();

       protected:
        void _markPotentiallyDirty();

       private:
        void _notifySubscribers(const bool& isDirty);
    };

    /**
     * @brief State of an item in the store
     *
     */
    enum class StoreState : std::uint8_t
    {
        Clean,
        New,
        Modified,
        Deleted
    };

}   // namespace app

#endif   // __APP__STORE__I_STORE_HPP__
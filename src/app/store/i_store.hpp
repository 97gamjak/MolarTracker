#ifndef __APP__STORE__I_STORE_HPP__
#define __APP__STORE__I_STORE_HPP__

#include <cstdint>

#include "connections/observable.hpp"

class Connection;   // Forward declaration

namespace app
{
    /**
     * @brief Tag struct for indicating that the dirty state of a store has
     * changed, this is used for notifying subscribers when the dirty state of a
     * store changes, the TagType is a boolean indicating whether the store is
     * dirty (has unsaved changes) or not
     *
     */
    struct DirtyChanged
    {
        /// Type alias for the type of data
        using TagType = bool;
    };

    /**
     * @brief Interface for a generic store
     *
     */
    class IStore : public Observable<DirtyChanged>
    {
       private:
        /// Type alias for the base class
        using Base = Observable<DirtyChanged>;

        /// Type alias for the change callback function for the dirty state of
        /// the store
        using DirtyChangedFn = void (*)(void*, const bool& isDirty);

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

        Connection subscribeToDirty(DirtyChangedFn func, void* user);

       protected:
        void _markPotentiallyDirty();
        void _clearPotentiallyDirty();

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
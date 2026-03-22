#ifndef __APP__STORE__BASE__BASE_STORE_HPP__
#define __APP__STORE__BASE__BASE_STORE_HPP__

#include <optional>
#include <vector>

#include "config/signal_tags.hpp"
#include "connections/observable.hpp"
#include "i_store.hpp"
#include "predicate.hpp"
#include "store_state.hpp"

namespace app
{

    /**
     * @brief BaseStore is a base class for stores that manage a collection of
     * entries. It provides common functionality for adding, removing, and
     * querying entries, as well as tracking the dirty state of the store.
     *
     * Template Parameters:
     * - T: The type of the value stored in each entry.
     * - IdType: The type used for identifying entries (e.g., int, std::string).
     *
     * The Entry struct represents an individual entry in the store, containing
     * a value of type T and a StoreState indicating whether the entry is clean,
     * dirty, or deleted.
     *
     * The BaseStore class inherits from Observable<OnDirtyChanged> to allow
     * subscribers to be notified when the dirty state changes, and from IStore
     * to provide a common interface for all stores.
     */
    // TODO (97gamjak)[MOLTRACK-206]: introduce concepts for types containing
    // Ids and ids
    template <typename T, typename IdType>
    class BaseStore : public Observable<OnDirtyChanged>, public IStore
    {
       public:
        /// Helper type for the dirty state change signal.
        using DirtyObservable = Observable<OnDirtyChanged>;

        /// struct representing an entry in the store, containing a value and
        /// its state.
        struct Entry;

       private:
        /// The collection of entries in the store.
        std::vector<Entry> _entries;

        /// Flag indicating whether the store is potentially dirty (i.e., has
        /// unsaved changes).
        bool _isPotentiallyDirty = false;

       public:
        BaseStore()                       = default;
        ~BaseStore() override             = default;
        BaseStore(BaseStore&&)            = default;
        BaseStore& operator=(BaseStore&&) = default;

        // Deleted copy constructor and copy assignment operator to prevent
        // copying of the store, as it manages a collection of entries and
        // copying could lead to issues with shared state and dirty tracking.
        BaseStore(const BaseStore&)            = delete;
        BaseStore& operator=(const BaseStore&) = delete;

        [[nodiscard]] bool isDirty() const override;

        void clearPotentiallyDirty() override;

       protected:
        [[nodiscard]] bool _isDeleted(IdType id) const;
        [[nodiscard]] bool _hasNonDeletedEntries() const;

        [[nodiscard]] const std::vector<Entry>& _getEntries() const;
        [[nodiscard]] std::vector<Entry>&       _getEntries();

        [[nodiscard]] Entry*           _findEntry(IdType id);
        [[nodiscard]] Entry*           _findEntry(Predicate<Entry> pred);
        [[nodiscard]] std::optional<T> _get(Predicate<Entry> pred) const;

        void _addEntry(const T& value, StoreState state);
        void _removeEntry(IdType id);
        void _cleanEntries();
        void _clearEntries();

        [[nodiscard]] IdType _generateNewId() const;

        void _markPotentiallyDirty();

        [[nodiscard]] Connection subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        ) override;
    };

    /**
     * @brief Struct representing an entry in the BaseStore, containing a value
     * of type T and its state.
     *
     * @tparam T
     * @tparam IdType
     */
    template <typename T, typename IdType>
    struct BaseStore<T, IdType>::Entry
    {
        /// The value stored in the entry.
        T value;

        /// The state of the entry (e.g., Clean, New, Modified, Deleted).
        StoreState state;
    };

}   // namespace app

#ifndef __APP__STORE__BASE__BASE_STORE_TPP__
#include "base_store.tpp"
#endif   // __APP__STORE__BASE__BASE_STORE_TPP__

#endif   // __APP__STORE__BASE__BASE_STORE_HPP__
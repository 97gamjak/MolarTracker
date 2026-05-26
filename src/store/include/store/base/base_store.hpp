#ifndef __STORE__INCLUDE__STORE__BASE__BASE_STORE_HPP__
#define __STORE__INCLUDE__STORE__BASE__BASE_STORE_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <optional>
#include <unordered_map>
#include <vector>

#include "config/signal_tags.hpp"
#include "config/strong_id.hpp"
#include "connections/observable.hpp"
#include "filter/predicate.hpp"
#include "store/i_store.hpp"
#include "store_state.hpp"

namespace store
{

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define STATE_POLICY_LIST(X) \
    X(IncludeDelete)         \
    X(ExcludeDelete)         \
    X(OnlyDeleted)

    MSTD_ENUM(DeletionPolicy, std::int8_t, STATE_POLICY_LIST);

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define STORE_RESULT_LIST(X) \
    X(Ok)                    \
    X(NotFound)

    MSTD_ENUM(StoreResult, std::int8_t, STORE_RESULT_LIST);

    /**
     * @brief Struct representing filter options for querying entries in the
     * store. This can be extended in the future to include additional options
     * for filtering entries based on specific criteria.
     *
     * @tparam T
     * @tparam IdType
     */
    template <typename T, typename IdType>
    struct FilterOptions
    {
        /// A predicate used to filter entries in the store
        filter::Predicate<T> filter = filter::Predicate<T>();

        /// A policy for including or excluding deleted entries in the results
        DeletionPolicy deletion = DeletionPolicy::IncludeDelete;

        /**
         * @brief evaluates whether an entry matches the filter options, this is
         * used to determine whether an entry should be included in the results
         * of a query based on the specified filter predicate and deletion
         * policy, allowing for flexible querying of entries in the store based
         * on various criteria defined in the filter and whether deleted entries
         * should be included or excluded according to the deletion policy.
         *
         * @tparam U
         * @param entry
         * @return true
         * @return false
         */
        // TODO(97gamjak): move this to the .tpp file
        template <typename U>
        bool eval(const U& entry) const
        {
            return filter::evaluatePredicate(filter, entry.value) &&
                   (deletion == DeletionPolicy::IncludeDelete ||
                    entry.state != StoreState::Deleted);
        }
    };

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
    class BaseStore : public IStore,
                      public Observable<
                          OnDirtyChanged,
                          OnStoreItemAdded<T>,
                          OnStoreItemUpdated<T>,
                          OnStoreItemRemoved<IdType>,
                          OnIdRemap<IdType>,
                          StoreChanged<IdType>>

    {
       public:
        /// Helper type for the dirty state change signal.
        using StoreObservable = Observable<
            OnDirtyChanged,
            OnStoreItemAdded<T>,
            OnStoreItemUpdated<T>,
            OnStoreItemRemoved<IdType>,
            OnIdRemap<IdType>,
            StoreChanged<IdType>>;

        /// Type alias for filter options used when querying entries in the
        /// store.
        using Options = FilterOptions<T, IdType>;

        /// struct representing an entry in the store, containing a value and
        /// its state.
        struct Entry;

        /// Type alias for the ID map used to track ID remappings.
        using IdMap = std::unordered_map<IdType, IdType, typename IdType::Hash>;

       private:
        /// The collection of entries in the store.
        std::vector<Entry> _entries;

        /// Flag indicating whether the store is potentially dirty (i.e., has
        /// unsaved changes).
        bool _isPotentiallyDirty = false;

        /// Map for remapping IDs
        IdMap _idRemap;
        /// Vector for tracking updated entries
        std::vector<T> _updated;
        /// Vector for tracking added entries
        std::vector<T> _added;
        /// Vector for tracking removed entry IDs
        std::vector<IdType> _removed;

        /// Sequence for generating new IDs
        IdSequence<IdType> _idSequence;

        /// Flag indicating whether the store has already notified subscribers
        bool _alreadyNotified = false;

        /// Flag indicating whether the store is fully cached
        bool _fullCache = false;

       public:
        BaseStore() = default;
        explicit BaseStore(bool fullCache);

        [[nodiscard]] bool isDirty() const override;
        [[nodiscard]] bool allDirty() const;

        void clearPotentiallyDirty() override;

        [[nodiscard]] Connection subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        ) override;

        // cppcheck-suppress functionConst -- false positive
        [[nodiscard]] Connection subscribeToIdRemap(
            OnIdRemap<IdType>::func func,
            void*                   user
        );

        // cppcheck-suppress functionConst -- false positive
        [[nodiscard]] Connection subscribeToEntryRemoved(
            OnStoreItemRemoved<IdType>::func func,
            void*                            user
        );

        // cppcheck-suppress functionConst -- false positive
        [[nodiscard]] Connection subscribeToEntryAdded(
            OnStoreItemAdded<T>::func func,
            void*                     user
        );

        // cppcheck-suppress functionConst -- false positive
        [[nodiscard]] Connection subscribeToEntryUpdated(
            OnStoreItemUpdated<T>::func func,
            void*                       user
        );

        // cppcheck-suppress functionConst -- false positive
        [[nodiscard]] Connection subscribeToStoreChange(
            StoreChanged<IdType>::func func,
            void*                      user
        );

        [[nodiscard]]
        bool isFullCache() const;

       protected:
        [[nodiscard]] bool _isDeleted(IdType id) const;
        [[nodiscard]] bool _hasNonDeletedEntries() const;

        [[nodiscard]]
        auto _getEntries(Options options = Options()) const;
        [[nodiscard]]
        auto _getValues(Options options = Options()) const;
        [[nodiscard]]
        std::optional<T> _get(Options options = Options()) const;
        [[nodiscard]]
        auto _getEntry(Options options = Options()) const;
        [[nodiscard]]
        idSet<IdType> _getIds(Options options = Options()) const;

        IdType      _addEntry(T value);
        void        _addCleanEntries(const std::vector<T>& value);
        StoreResult _updateEntry(const T& value, StoreState state);
        StoreResult _commitEntry(IdType tempId, const Entry& persistedValue);
        StoreResult _removeEntry(IdType id);
        StoreResult _deleteEntry(IdType id);

        void _clearEntries();

        void _notifyOnCommit();

        [[nodiscard]] const IdMap& _getIdRemap() const;

       private:
        static bool _evalDeletionPolicy(
            const Entry&   entry,
            DeletionPolicy policy
        );

        [[nodiscard]]
        Entry* _findEntry(IdType id);

        void                 _markPotentiallyDirty();
        [[nodiscard]] IdType _generateNewId();

        void _notifyIdRemap(bool checkAlreadyNotified);
        void _notifyUpdated(bool checkAlreadyNotified);
        void _notifyAdded(bool checkAlreadyNotified);
        void _notifyRemoved(bool checkAlreadyNotified);
        void _notifyStoreChanged(bool checkAlreadyNotified);
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

}   // namespace store

#ifndef __STORE__INCLUDE__STORE__BASE__BASE_STORE_TPP__
#include "base_store.tpp"
#endif
#ifndef __STORE__INCLUDE__STORE__BASE__BASE_STORE_SUBSCRIPTIONS_TPP__
#include "base_store_subscriptions.tpp"   // IWYU pragma: keep
#endif
#ifndef __STORE__INCLUDE__STORE__BASE__BASE_STORE_NOTIFICATIONS_TPP__
#include "base_store_notifications.tpp"   // IWYU pragma: keep
#endif

#endif   // __STORE__INCLUDE__STORE__BASE__BASE_STORE_HPP__
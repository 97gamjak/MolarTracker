#ifndef __APP__INCLUDE__APP__STORE__BASE__BASE_STORE_HPP__
#define __APP__INCLUDE__APP__STORE__BASE__BASE_STORE_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <optional>
#include <unordered_map>
#include <vector>

#include "config/signal_tags.hpp"
#include "connections/observable.hpp"
#include "filter/predicate.hpp"
#include "i_store.hpp"
#include "store_state.hpp"

namespace app
{

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define STATE_POLICY_LIST(X) \
    X(IncludeDelete)         \
    X(ExcludeDelete)         \
    X(OnlyDeleted)

    MSTD_ENUM(DeletionPolicy, std::int8_t, STATE_POLICY_LIST);

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
    class BaseStore : public Observable<OnDirtyChanged>, public IStore
    {
       public:
        /// Helper type for the dirty state change signal.
        using DirtyObservable = Observable<OnDirtyChanged>;

        /// Type alias for a map of IDs.
        using IdMap = std::unordered_map<IdType, IdType, typename IdType::Hash>;

        /// Type alias for filter options used when querying entries in the
        /// store.
        using Options = FilterOptions<T, IdType>;

        /// struct representing an entry in the store, containing a value and
        /// its state.
        struct Entry;

       private:
        /// The collection of entries in the store.
        std::vector<Entry> _entries;

        /// A mapping of old IDs to new IDs for entries that have been updated
        IdMap _changedIds;

        /// Flag indicating whether the store is potentially dirty (i.e., has
        /// unsaved changes).
        bool _isPotentiallyDirty = false;

       public:
        BaseStore()           = default;
        ~BaseStore() override = default;

        /// @cond DOXYGEN_IGNORE
        BaseStore(BaseStore&&)            = default;
        BaseStore& operator=(BaseStore&&) = default;
        /// @endcond

        // Deleted copy constructor and copy assignment operator to prevent
        // copying of the store, as it manages a collection of entries and
        // copying could lead to issues with shared state and dirty tracking.
        BaseStore(const BaseStore&)            = delete;
        BaseStore& operator=(const BaseStore&) = delete;

        [[nodiscard]] bool isDirty() const override;
        [[nodiscard]] bool allDirty() const;

        void clearPotentiallyDirty() override;

        [[nodiscard]] const IdMap& getChangedIds() const;

       protected:
        [[nodiscard]] bool _isDeleted(IdType id) const;
        [[nodiscard]] bool _hasNonDeletedEntries() const;

        [[nodiscard]]
        auto _getEntries(Options options = Options()) const;
        [[nodiscard]]
        auto _getEntries(Options options = Options());
        [[nodiscard]]
        auto _getValues(Options options = Options()) const;

        [[nodiscard]]
        Entry* _findEntry(Options options = Options());
        [[nodiscard]]
        std::optional<T> _get(Options options = Options()) const;

        void _addEntry(const T& value, StoreState state);
        void _removeEntry(IdType id);
        void _cleanEntries();
        void _clearEntries();

        void _appendChangedIds(IdType oldId, IdType newId);
        void _clearChangedIds();

        [[nodiscard]] IdType _generateNewId() const;

        void _markPotentiallyDirty();

        [[nodiscard]] Connection subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        ) override;

       private:
        static bool _evalDeletionPolicy(
            const Entry&   entry,
            DeletionPolicy policy
        );
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

#ifndef __APP__INCLUDE__APP__STORE__BASE__BASE_STORE_TPP__
#include "base_store.tpp"
#endif

#endif   // __APP__INCLUDE__APP__STORE__BASE__BASE_STORE_HPP__
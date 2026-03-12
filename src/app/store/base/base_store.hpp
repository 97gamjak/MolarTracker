#ifndef __APP__STORE__BASE__BASE_STORE_HPP__
#define __APP__STORE__BASE__BASE_STORE_HPP__

#include <functional>
#include <optional>
#include <vector>

#include "config/signal_tags.hpp"
#include "connections/observable.hpp"
#include "i_store.hpp"
#include "predicate.hpp"
#include "store_state.hpp"

namespace app
{

    template <typename T, typename IdType>
    class BaseStore : public Observable<OnDirtyChanged>, public IStore
    {
       public:
        using DirtyObservable = Observable<OnDirtyChanged>;
        struct Entry;

       private:
        std::vector<Entry> _entries;

        bool _isPotentiallyDirty = false;

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

       public:
        [[nodiscard]] bool isDirty() const override;

        void clearPotentiallyDirty() override;
    };

    template <typename T, typename IdType>
    struct BaseStore<T, IdType>::Entry
    {
        T          value;
        StoreState state;
    };

}   // namespace app

#ifndef __APP__STORE__BASE__BASE_STORE_TPP__
#include "base_store.tpp"
#endif   // __APP__STORE__BASE__BASE_STORE_TPP__

#endif   // __APP__STORE__BASE__BASE_STORE_HPP__
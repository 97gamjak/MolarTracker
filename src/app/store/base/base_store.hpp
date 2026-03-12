#ifndef __APP__STORE__BASE__BASE_STORE_HPP__
#define __APP__STORE__BASE__BASE_STORE_HPP__

#include <vector>

#include "config/signal_tags.hpp"
#include "connections/observable.hpp"
#include "i_store.hpp"
#include "store_state.hpp"

namespace app
{

    template <typename T, typename IdType>
    class BaseStore : public Observable<OnDirtyChanged>, public IStore
    {
       public:
        using DirtyObservable = Observable<OnDirtyChanged>;

       protected:
        struct Entry;

       private:
        std::vector<Entry> _entries;

        bool _isPotentiallyDirty = false;

       protected:
        [[nodiscard]] bool _isDeleted(IdType id) const;

        [[nodiscard]] Entry* _findEntry(IdType id);
        [[nodiscard]] IdType _generateNewId() const;

        void _removeEntry(IdType id);

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
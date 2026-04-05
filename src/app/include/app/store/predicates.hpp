#ifndef __APP__INCLUDE__APP__STORE__PREDICATES_HPP__
#define __APP__INCLUDE__APP__STORE__PREDICATES_HPP__

#include "app/store/base/base_store.hpp"
#include "app/store/base/predicate.hpp"
#include "finance/account.hpp"

namespace app
{
    Predicate<BaseStore<finance::Account, AccountId>::Entry> HasAccountId(
        AccountId id
    );

    Predicate<BaseStore<finance::Account, AccountId>::Entry> IsExternal();

    Predicate<BaseStore<finance::Account, AccountId>::Entry> hasCurrency(
        Currency currency
    );

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__PREDICATES_HPP__

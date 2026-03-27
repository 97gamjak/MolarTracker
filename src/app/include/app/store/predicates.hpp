#ifndef __APP__INCLUDE__APP__STORE__PREDICATES_HPP__
#define __APP__INCLUDE__APP__STORE__PREDICATES_HPP__

#include "app/store/base/base_store.hpp"
#include "app/store/base/predicate.hpp"
#include "finance/cash_account.hpp"

namespace app
{
    Predicate<BaseStore<finance::AccountVariant, AccountId>::Entry> HasAccountId(
        AccountId id
    );

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__PREDICATES_HPP__

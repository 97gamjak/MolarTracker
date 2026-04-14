#ifndef __APP__INCLUDE__APP__STORE__PREDICATES_HPP__
#define __APP__INCLUDE__APP__STORE__PREDICATES_HPP__

#include "app/store/base/predicate.hpp"
#include "finance/account.hpp"

namespace app
{
    Predicate<finance::Account> HasAccountId(AccountId id);

    Predicate<finance::Account> IsExternal();

    Predicate<finance::Account> HasCurrency(Currency currency);

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__PREDICATES_HPP__

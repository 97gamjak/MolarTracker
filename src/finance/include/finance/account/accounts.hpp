#ifndef __FINANCE__INCLUDE__FINANCE__ACCOUNT__ACCOUNTS_HPP__
#define __FINANCE__INCLUDE__FINANCE__ACCOUNT__ACCOUNTS_HPP__

#include "finance/account/account.hpp"
#include "utils/iterable.hpp"

namespace finance
{
    /**
     * @brief A collection of financial accounts.
     */
    class Accounts : public Iterable<Account>
    {
       public:
        using Iterable<Account>::Iterable;

        [[nodiscard]]
        idSet<AccountId> getIds() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__ACCOUNT__ACCOUNTS_HPP__

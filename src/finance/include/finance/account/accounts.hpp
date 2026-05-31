#ifndef __FINANCE__INCLUDE__FINANCE__ACCOUNT__ACCOUNTS_HPP__
#define __FINANCE__INCLUDE__FINANCE__ACCOUNT__ACCOUNTS_HPP__

#include "finance/account/account.hpp"
#include "utils/container/id_map.hpp"

namespace finance
{
    /**
     * @brief A collection of financial accounts.
     */
    class Accounts : public IdMap<Account>
    {
       public:
        using IdMap<Account>::IdMap;

        [[nodiscard]]
        Accounts filterExternal(bool external) const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__ACCOUNT__ACCOUNTS_HPP__

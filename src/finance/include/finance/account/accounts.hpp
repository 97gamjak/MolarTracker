#ifndef __FINANCE__INCLUDE__FINANCE__ACCOUNT__ACCOUNTS_HPP__
#define __FINANCE__INCLUDE__FINANCE__ACCOUNT__ACCOUNTS_HPP__

#include "error/finance_error.hpp"
#include "finance/account/account.hpp"
#include "utils/container/id_map.hpp"
#include "utils/container/set.hpp"

namespace finance
{
    /**
     * @brief A collection of financial accounts.
     */
    class Accounts : public IdObjectMap<Account>
    {
       public:
        using IdObjectMap<Account>::IdObjectMap;

        [[nodiscard]]
        Accounts filterExternal(bool external) const;

        [[nodiscard]]
        std::vector<std::optional<bool>> isExternal(
            const IdSet<AccountId>& ids
        ) const;
    };

    /**
     * @brief A view of financial accounts, providing a read-only interface to
     * access account data.
     */
    class AccountsView : public IdObjectMap<std::shared_ptr<const Account>>
    {
       public:
        using IdObjectMap<std::shared_ptr<const Account>>::IdObjectMap;

        [[nodiscard]]
        AccountsView cash() const;

        [[nodiscard]]
        AccountsView securities() const;

        [[nodiscard]]
        Result<AccountId, FinanceError> getCorrespondingExternalAccountId(
            const AccountId& cashAccountId
        ) const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__ACCOUNT__ACCOUNTS_HPP__

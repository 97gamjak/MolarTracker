#include "finance/account/accounts.hpp"

namespace finance
{
    /**
     * @brief Get the IDs of all accounts.
     *
     * @return idSet<AccountId>
     */
    idSet<AccountId> Accounts::getIds() const
    {
        return Iterable<Account>::_getIds<AccountId, AccountId::Hash>();
    }

    Accounts Accounts::filterExternal(bool external) const
    {
        Accounts filtered;
        for (const auto& account : *this)
            if (account.isExternal() == external)
                filtered.add(account);

        return filtered;
    }
}   // namespace finance

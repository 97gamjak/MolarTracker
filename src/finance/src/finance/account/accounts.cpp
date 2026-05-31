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
}   // namespace finance

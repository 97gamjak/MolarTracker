#include "finance/account/accounts.hpp"

namespace finance
{
    /**
     * @brief Filter accounts based on whether they are external or not.
     *
     * @param external If true, returns only external accounts; if false,
     * returns only internal accounts.
     * @return Accounts A new Accounts object containing the filtered accounts.
     */
    Accounts Accounts::filterExternal(bool external) const
    {
        Accounts filtered;
        for (const auto& [id, account] : *this)
            if (account.isExternal() == external)
                filtered.addUnchecked(account);

        return filtered;
    }

}   // namespace finance

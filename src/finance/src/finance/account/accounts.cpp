#include "finance/account/accounts.hpp"

namespace finance
{
    /**
     * @brief Get the corresponding external account ID for a given internal
     * cash account ID.
     *
     * @param cashAccountId The internal cash account ID to find the
     * corresponding external account for.
     * @return AccountId The corresponding external account ID, or an invalid
     * AccountId if no corresponding external account is found.
     */
    Result<AccountId, FinanceError> AccountsView::
        getCorrespondingExternalAccountId(const AccountId& cashAccountId) const
    {
        for (const auto& [id, account] : *this)
        {
            if (!account->isExternal() &&
                account->getKind() == AccountKind::Cash &&
                account->getId() == cashAccountId)
            {
                // find the corresponding external account
                for (const auto& [externalId, externalAccount] : *this)
                {
                    if (externalAccount->isExternal())
                        return externalId;
                }
            }
        }

        return FinanceError{
            FinanceErrorType::AccountNotFound,
            "No corresponding external account found for cash account: " +
                cashAccountId.toString()
        };
    }

    /**
     * @brief Filter accounts to include only cash accounts.
     *
     * @return AccountsView A new AccountsView object containing only cash
     * accounts.
     */
    AccountsView AccountsView::cash() const
    {
        AccountsView filtered;
        for (const auto& [id, account] : *this)
            if (account->getKind() == AccountKind::Cash)
                filtered.addUnchecked(account);

        return filtered;
    }

    /**
     * @brief Filter accounts to include only security accounts.
     *
     * @return AccountsView A new AccountsView object containing only security
     * accounts.
     */
    AccountsView AccountsView::securities() const
    {
        AccountsView filtered;
        for (const auto& [id, account] : *this)
            if (account->getKind() == AccountKind::Security)
                filtered.addUnchecked(account);

        return filtered;
    }

    AccountsView AccountsView::removeExternal() const
    {
        AccountsView filtered;
        for (const auto& [id, account] : *this)
            if (!account->isExternal())
                filtered.addUnchecked(account);

        return filtered;
    }

    bool AccountsView::isExternal(const AccountId& accountId) const
    {
        if (contains(accountId))
        {
            const auto& account = at(accountId);
            return account->isExternal();
        }

        return false;
    }
}   // namespace finance

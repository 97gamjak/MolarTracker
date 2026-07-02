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

    /**
     * @brief Checks if the specified account IDs are external.
     *
     * @param ids The account IDs to check.
     * @return A vector of optional booleans indicating whether each account is
     * external (true), internal (false), or not found (nullopt).
     */
    std::vector<std::optional<bool>> Accounts::isExternal(
        const IdSet<AccountId>& ids
    ) const
    {
        std::vector<std::optional<bool>> results;
        results.reserve(ids.size());

        for (const auto& id : ids)
        {
            if (contains(id))
                results.emplace_back(at(id).isExternal());
            else
                results.emplace_back(std::nullopt);
        }

        return results;
    }

    /**
     * @brief Get the corresponding external account ID for a given internal
     * cash account ID.
     *
     * @param cashAccountId The internal cash account ID to find the
     * corresponding external account for.
     * @return AccountId The corresponding external account ID, or an invalid
     * AccountId if no corresponding external account is found.
     */
    AccountId Accounts::getCorrespondingExternalAccountId(
        const AccountId& cashAccountId
    ) const
    {
        for (const auto& [id, account] : *this)
        {
            if (!account.isExternal() &&
                account.getKind() == AccountKind::Cash &&
                account.getId() == cashAccountId)
            {
                // find the corresponding external account
                for (const auto& [externalId, externalAccount] : *this)
                {
                    if (externalAccount.isExternal())
                        return externalId;
                }
            }
        }

        return AccountId::invalid();
    }
}   // namespace finance

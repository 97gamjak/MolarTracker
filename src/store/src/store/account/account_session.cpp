#include "store/account/account_session.hpp"

#include <algorithm>

#include "config/strong_id.hpp"
#include "finance/account.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("Store.Account.AccountSession");

namespace store
{
    /**
     * @brief set the active accounts in the session, this will replace the
     * current set of active accounts with the provided set
     *
     * @param activeAccounts
     */
    void AccountSession::set(
        const std::vector<finance::Account>& activeAccounts
    )
    {
        LOG_DEBUG(
            std::format("Setting active accounts: {}", activeAccounts.size())
        );
        _activeAccounts = activeAccounts;
    }

    /**
     * @brief add accounts to the session, this will add the provided accounts
     * to the current set of active accounts in the session
     *
     * @param accounts
     */
    void AccountSession::add(const std::vector<finance::Account>& accounts)
    {
        for (const auto& account : accounts)
            if (!contains(account))
                _activeAccounts.push_back(account);
    }

    /**
     * @brief remove accounts from the session, this will remove the provided
     * account IDs from the current set of active accounts in the session
     *
     * @param accounts
     */
    void AccountSession::remove(const std::vector<finance::Account>& accounts)
    {
        std::erase_if(
            _activeAccounts,
            [&](const auto& account)
            {
                return std::ranges::any_of(
                    accounts,
                    [&](const auto& other)
                    { return account.getId() == other.getId(); }
                );
            }
        );
    }

    void AccountSession::remove(const std::vector<AccountId>& ids)
    {
        std::erase_if(
            _activeAccounts,
            [&](const auto& account)
            {
                return std::ranges::any_of(
                    ids,
                    [&](const auto& id) { return account.getId() == id; }
                );
            }
        );
    }

    const std::vector<finance::Account>& AccountSession::getAccounts() const
    {
        return _activeAccounts;
    }

    /**
     * @brief get the current set of active account IDs in the session
     *
     * @return const idSet<AccountId>& A reference to the set of active account
     * IDs in the session
     */
    idSet<AccountId> AccountSession::getIds() const
    {
        idSet<AccountId> ids;
        for (const auto& account : _activeAccounts)
            ids.insert(account.getId());

        return ids;
    }

    idSet<AccountId> AccountSession::getNonExternalIds() const
    {
        idSet<AccountId> nonExternalIds;
        for (const auto& account : _activeAccounts)
            if (!account.isExternal())
                nonExternalIds.insert(account.getId());

        return nonExternalIds;
    }

    bool AccountSession::contains(const finance::Account& account) const
    {
        return std::ranges::any_of(
            _activeAccounts,
            [&](const auto& account_)
            { return account_.getId() == account.getId(); }
        );
    }
}   // namespace store
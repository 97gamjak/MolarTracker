#include "store/account/account_session.hpp"

#include "finance/account.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("App.Store.Account.AccountSession");

namespace store
{
    /**
     * @brief set the active accounts in the session, this will replace the
     * current set of active accounts with the provided set
     *
     * @param activeAccounts
     */
    void AccountSession::set(const idSet<AccountId>& activeAccounts)
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
            _activeAccounts.insert(account.getId());
    }

    /**
     * @brief remove accounts from the session, this will remove the provided
     * account IDs from the current set of active accounts in the session
     *
     * @param accountIds
     */
    void AccountSession::remove(const std::vector<AccountId>& accountIds)
    {
        for (const auto& accountId : accountIds)
            _activeAccounts.erase(accountId);
    }

    /**
     * @brief get the current set of active account IDs in the session
     *
     * @return const idSet<AccountId>& A reference to the set of active account
     * IDs in the session
     */
    const idSet<AccountId>& AccountSession::getIds() const
    {
        return _activeAccounts;
    }
}   // namespace store
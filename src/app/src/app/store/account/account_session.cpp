#include "app/store/account/account_session.hpp"

#include "finance/account.hpp"

namespace app
{
    void AccountSession::set(const idSet<AccountId>& activeAccounts)
    {
        _activeAccounts = activeAccounts;
    }

    void AccountSession::add(const std::vector<finance::Account>& accounts)
    {
        for (const auto& account : accounts)
            _activeAccounts.insert(account.getId());
    }

    void AccountSession::remove(const std::vector<AccountId>& accountIds)
    {
        for (const auto& accountId : accountIds)
            _activeAccounts.erase(accountId);
    }

    const idSet<AccountId>& AccountSession::getIds() const
    {
        return _activeAccounts;
    }
}   // namespace app
#include "cache/account_cache.hpp"

namespace cache
{
    MTResult<std::shared_ptr<const finance::Account>, FinanceError> AccountCacheUtils::
        getCashAccount(
            const AccountId&                     accountId,
            const std::shared_ptr<AccountCache>& accountCache
        )
    {
        return _getAccount(accountId, AccountKind::Cash, accountCache);
    }

    MTResult<std::shared_ptr<const finance::Account>, FinanceError> AccountCacheUtils::
        getSecurityAccount(
            const AccountId&                     accountId,
            const std::shared_ptr<AccountCache>& accountCache
        )
    {
        return _getAccount(accountId, AccountKind::Security, accountCache);
    }

    MTResult<std::shared_ptr<const finance::Account>, FinanceError> AccountCacheUtils::
        _getAccount(
            const AccountId&                     accountId,
            const AccountKind&                   kind,
            const std::shared_ptr<AccountCache>& accountCache
        )
    {
        const auto& account = accountCache->get(accountId);

        if (account == nullptr)
        {
            return FinanceError(
                FinanceErrorType::AccountNotFound,
                std::format(
                    "Account with ID {} not found in cache",
                    accountId.toString()
                )
            );
        }

        if (account->getKind() != kind)
        {
            return FinanceError(
                FinanceErrorType::InvalidAccount,
                std::format(
                    "Account with ID {} is not of kind {}",
                    accountId.toString(),
                    AccountKindMeta::toString(kind)
                )
            );
        }

        return account;
    }
}   // namespace cache
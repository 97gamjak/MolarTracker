#include "cache/account_cache.hpp"

namespace cache
{
    /**
     * @brief Get a cash account from the cache, this method retrieves a cash
     * account by its ID from the provided account cache, performing validation
     * to ensure that the account exists and is of the correct kind (cash).
     *
     * @param accountId The ID of the cash account to retrieve.
     * @param accountCache A shared pointer to the AccountCache instance from
     * which to retrieve the account.
     *
     * @return FinanceResult<std::shared_ptr<const finance::Account>> A
     * result containing a shared pointer to the cash account if found and
     * valid, or a FinanceError indicating the reason for failure (e.g., account
     * not found or invalid account kind).
     */
    FinanceResult<std::shared_ptr<const finance::Account>> AccountCacheUtils::
        getCashAccount(
            const AccountId&                     accountId,
            const std::shared_ptr<AccountCache>& accountCache
        )
    {
        return getAccount(accountId, accountCache, AccountKind::Cash);
    }

    /**
     * @brief Get a security account from the cache, this method retrieves a
     * security account by its ID from the provided account cache, performing
     * validation to ensure that the account exists and is of the correct kind
     * (security).
     *
     * @param accountId The ID of the security account to retrieve.
     * @param accountCache A shared pointer to the AccountCache instance from
     * which to retrieve the account.
     *
     * @return FinanceResult<std::shared_ptr<const finance::Account>> A
     * result containing a shared pointer to the security account if found and
     * valid, or a FinanceError indicating the reason for failure (e.g., account
     * not found or invalid account kind).
     */
    FinanceResult<std::shared_ptr<const finance::Account>> AccountCacheUtils::
        getSecurityAccount(
            const AccountId&                     accountId,
            const std::shared_ptr<AccountCache>& accountCache
        )
    {
        return getAccount(accountId, accountCache, AccountKind::Security);
    }

    /**
     * @brief Get an account from the cache, this method retrieves an account by
     * its ID from the provided account cache, performing validation to ensure
     * that the account exists and optionally checking if it is of a specified
     * kind (cash or security).
     *
     * @param accountId The ID of the account to retrieve.
     * @param accountCache A shared pointer to the AccountCache instance from
     * which to retrieve the account.
     * @param kind An optional parameter specifying the expected kind of the
     * account (cash or security). If provided, the method will validate that
     * the retrieved account matches this kind.
     *
     * @return FinanceResult<std::shared_ptr<const finance::Account>> A
     * result containing a shared pointer to the account if found and valid, or
     * a FinanceError indicating the reason for failure (e.g., account not
     * found, invalid account kind).
     */
    FinanceResult<std::shared_ptr<const finance::Account>> AccountCacheUtils::
        getAccount(
            const AccountId&                     accountId,
            const std::shared_ptr<AccountCache>& accountCache,
            const std::optional<AccountKind>&    kind
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

        if (kind.has_value() && account->getKind() != kind.value())
        {
            return FinanceError(
                FinanceErrorType::InvalidAccount,
                std::format(
                    "Account with ID {} is not of kind {}",
                    accountId.toString(),
                    AccountKindMeta::toString(kind.value())
                )
            );
        }

        return account;
    }
}   // namespace cache
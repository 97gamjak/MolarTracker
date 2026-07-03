#ifndef __CACHE__INCLUDE__CACHE__ACCOUNT_CACHE_HPP__
#define __CACHE__INCLUDE__CACHE__ACCOUNT_CACHE_HPP__

#include "config/error.hpp"
#include "finance/account/account.hpp"
#include "single_cache.hpp"
#include "utils/finance.hpp"

namespace store
{
    class IAccountStoreReader;   // forward declaration
}   // namespace store

namespace cache
{
    /**
     * @brief A cache for storing accounts, this class is responsible for
     * managing the cache of account objects, allowing for efficient retrieval
     * and management of cached account data. It provides methods for getting
     * accounts by their identifiers, as well as tracking cache statistics such
     * as hits, misses, and evictions.
     *
     */
    class AccountCache : public SingleCache<AccountId, finance::Account>
    {
       private:
        /// Shared pointer to the account store
        std::shared_ptr<store::IAccountStoreReader> _reader;

       public:
        explicit AccountCache(
            const std::shared_ptr<store::IAccountStoreReader>& reader
        );

       protected:
        [[nodiscard]]
        std::shared_ptr<const finance::Account> _load(
            const AccountId& accountId
        ) override;
    };

    class AccountCacheUtils
    {
       public:
        static MTResult<std::shared_ptr<const finance::Account>, FinanceError> getCashAccount(
            const AccountId&                     accountId,
            const std::shared_ptr<AccountCache>& accountCache
        );

        static MTResult<std::shared_ptr<const finance::Account>, FinanceError> getSecurityAccount(
            const AccountId&                     accountId,
            const std::shared_ptr<AccountCache>& accountCache
        );

        static MTResult<std::shared_ptr<const finance::Account>, FinanceError> _getAccount(
            const AccountId&                     accountId,
            const AccountKind&                   kind,
            const std::shared_ptr<AccountCache>& accountCache
        );
    };

}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__ACCOUNT_CACHE_HPP__
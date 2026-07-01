#ifndef __CACHE__INCLUDE__CACHE__ACCOUNT_CACHE_HPP__
#define __CACHE__INCLUDE__CACHE__ACCOUNT_CACHE_HPP__

#include "finance/account/account.hpp"
#include "single_cache.hpp"

namespace store
{
    class IAccountStore;   // forward declaration
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
        std::shared_ptr<store::IAccountStore> _reader;

       public:
        explicit AccountCache(
            const std::shared_ptr<store::IAccountStore>& reader
        );
    };

}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__ACCOUNT_CACHE_HPP__
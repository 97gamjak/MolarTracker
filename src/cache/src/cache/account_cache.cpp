#include "cache/account_cache.hpp"

namespace cache
{
    /**
     * @brief Construct a new AccountCache:: AccountCache object
     *
     * @param reader Shared pointer to the account store reader
     */
    AccountCache::AccountCache(
        const std::shared_ptr<store::IAccountStore>& reader
    )
        : _reader(reader)
    {
    }

}   // namespace cache
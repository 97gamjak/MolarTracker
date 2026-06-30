#include "cache/base_cache.hpp"

namespace cache
{
    /**
     * @brief get the hit rate of the cache, calculated as the ratio of hits to
     * the total number of accesses (hits + misses).
     *
     * @return double
     */
    double CacheStats::hitRate() const
    {
        auto total = hits + misses;
        return total > 0
                   ? static_cast<double>(hits) / static_cast<double>(total)
                   : 0.0;
    }
}   // namespace cache
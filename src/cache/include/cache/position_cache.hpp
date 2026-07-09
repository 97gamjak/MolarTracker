#ifndef __CACHE__INCLUDE__CACHE__POSITION_CACHE_HPP__
#define __CACHE__INCLUDE__CACHE__POSITION_CACHE_HPP__

#include <memory>

#include "cache/single_cache.hpp"
#include "finance/position.hpp"

namespace store
{
    class IPositionStoreReader;   // forward declaration
}   // namespace store

namespace finance
{
    struct PositionFilter;   // forward declaration
}   // namespace finance

namespace cache
{
    class PositionCache : public SingleCache<PositionId, finance::Position>
    {
       private:
        std::shared_ptr<store::IPositionStoreReader> _reader;

        bool _openPositionsLoaded = false;

       public:
        explicit PositionCache(
            const std::shared_ptr<store::IPositionStoreReader>& reader
        );

        [[nodiscard]]
        std::vector<finance::PositionView> getPositions(
            const finance::PositionFilter& filter
        );

       protected:
        [[nodiscard]]
        finance::PositionView _load(const PositionId& key) override;
    };

    class PositionCacheUtils
    {
       public:
        Set<std::string> getOpenPositionSymbols(
            const std::shared_ptr<PositionCache>& positionCache,
            const IdSet<PositionId>&              positionIds
        );
    };
}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__POSITION_CACHE_HPP__
#include "cache/position_cache.hpp"

#include "finance/position_filter.hpp"
#include "store/i_position_store.hpp"

namespace cache
{
    PositionCache::PositionCache(
        const std::shared_ptr<store::IPositionStoreReader>& reader
    )
        : _reader(reader)
    {
    }

    finance::PositionView PositionCache::_load(const PositionId& key)
    {
        const auto position = _reader->getPosition(key);

        if (position)
            return std::make_shared<const finance::Position>(position.value());

        return nullptr;
    }

    std::vector<finance::PositionView> PositionCache::getPositions(
        const finance::PositionFilter& filter
    )
    {
        std::vector<finance::PositionView> positions;

        bool getClosedPositionsOnly = false;
        // TODO: put this logic in a helper function to avoid duplication
        getClosedPositionsOnly |=
            filter.isOpen.has_value() && !filter.isOpen.value();
        getClosedPositionsOnly |= !filter.isOpen.has_value();

        if (filter.isOpen.has_value() && filter.isOpen && _openPositionsLoaded)
            return _getEntries().getValues();

        if (!getClosedPositionsOnly)
            _openPositionsLoaded = true;

        for (const auto& [id, position] : _reader->getPositions(filter))
        {
            if (!find(id))
            {
                const auto& posView =
                    std::make_shared<const finance::Position>(position);
                _addAndNotify(id, posView);
                positions.push_back(posView);
            }
        }

        return positions;
    }

}   // namespace cache
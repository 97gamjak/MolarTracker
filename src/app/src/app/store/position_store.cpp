#include "app/store/position_store.hpp"

#include "app/store/base/base_store.hpp"

namespace app
{

    /**
     * @brief Construct a new Position Store:: Position Store object
     *
     * @param positionService
     */
    PositionStore::PositionStore(
        std::shared_ptr<IPositionService> positionService
    )
        : _positionService(std::move(positionService))
    {
        const auto openPositions = _positionService->getAllOpenPositions();

        _addCleanEntries(openPositions);
    }

    /**
     * @brief Create a new position
     *
     * @param position
     * @return PositionId
     */
    PositionId PositionStore::createPosition(const finance::Position& position)
    {
        _addEntry(position);

        return position.getId();
    }

    /**
     * @brief Get all positions
     *
     * @return std::vector<finance::Position>
     */
    std::vector<finance::Position> PositionStore::getAllPositions()
    {
        auto options = Options{.deletion = DeletionPolicy::ExcludeDelete};

        auto positions = _getValues(options);

        return {positions.begin(), positions.end()};
    }

}   // namespace app

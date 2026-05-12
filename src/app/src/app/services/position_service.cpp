#include "position_service.hpp"

#include "app/repos_api/i_position_repo.hpp"
#include "finance/position.hpp"

namespace app
{
    /**
     * @brief Construct a new Position Service:: Position Service object
     *
     * @param positionRepo
     */
    PositionService::PositionService(
        std::shared_ptr<IPositionRepo> positionRepo
    )
        : _positionRepo(std::move(positionRepo))
    {
    }

    /**
     * @brief Create a new Position
     *
     * @param position
     * @return PositionId
     */
    PositionId PositionService::createPosition(
        const finance::Position& position
    )
    {
        return _positionRepo->createPosition(position);
    }

    /**
     * @brief Get all Positions
     *
     * @return std::vector<finance::Position>
     */
    std::vector<finance::Position> PositionService::getAllPositions()
    {
        return _positionRepo->getAllPositions();
    }

    /**
     * @brief Get all open Positions
     *
     * @return std::vector<finance::Position>
     */
    std::vector<finance::Position> PositionService::getAllOpenPositions()
    {
        return _positionRepo->getAllOpenPositions();
    }

}   // namespace app
#include "position_service.hpp"

#include "finance/position.hpp"
#include "logging/log_macros.hpp"
#include "repo/i_position_repo.hpp"

REGISTER_LOG_CATEGORY("Service.PositionService");

namespace service
{
    /**
     * @brief Construct a new Position Service:: Position Service object
     *
     * @param positionRepo
     */
    PositionService::PositionService(
        std::shared_ptr<repo::IPositionRepo> positionRepo
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
     * @param accountIds The IDs of the accounts to retrieve positions for.
     *
     * @return std::vector<finance::Position>
     */
    std::vector<finance::Position> PositionService::getAllPositions(
        const idSet<AccountId>& accountIds
    )
    {
        const auto positions = _positionRepo->getAllPositions(accountIds);

        LOG_DEBUG(std::format("Retrieved positions: {}", positions.size()));

        return positions;
    }

    /**
     * @brief Get all open Positions
     *
     * @param accountIds The IDs of the accounts to retrieve positions for.
     *
     * @return std::vector<finance::Position>
     */
    std::vector<finance::Position> PositionService::getAllOpenPositions(
        const idSet<AccountId>& accountIds
    )
    {
        const auto positions = _positionRepo->getAllOpenPositions(accountIds);

        LOG_DEBUG(
            std::format("Retrieved open positions: {}", positions.size())
        );

        return positions;
    }

}   // namespace service
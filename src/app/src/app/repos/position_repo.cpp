#include "position_repo.hpp"

#include <stdexcept>

#include "app/factories/position_factory.hpp"
#include "finance/position.hpp"
#include "orm/query_options.hpp"
#include "sql_models/position_row.hpp"

namespace app
{
    /**
     * @brief Create a new Position
     *
     * @param position
     * @return PositionId
     */
    PositionId PositionRepo::createPosition(const finance::Position& position)
    {
        const auto draft = PositionFactory::toPositionRow(position);

        auto result = _getCrud().insert(_getDb(), draft);

        if (!result)
        {
            throw std::runtime_error("Failed to create position");
        }

        return PositionId(result.value());
    }

    /**
     * @brief Get all Positions
     *
     * @return std::vector<finance::Position>
     */
    std::vector<finance::Position> PositionRepo::getAllPositions()
    {
        auto result = _getCrud().get<PositionRow>(_getDb());

        return PositionFactory::fromPositionRows(result);
    }

    /**
     * @brief Get all open Positions
     *
     * @return std::vector<finance::Position>
     */
    std::vector<finance::Position> PositionRepo::getAllOpenPositions()
    {
        const auto query = orm::Query{}.where(PositionRow::IsOpen());

        auto result = _getCrud().get<PositionRow>(_getDb(), query);

        return PositionFactory::fromPositionRows(result);
    }

}   // namespace app

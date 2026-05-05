#include "position_factory.hpp"

#include "finance/position.hpp"
#include "sql_models/position_row.hpp"

namespace app
{
    /**
     * @brief Create a Position from a PositionRow.
     *
     * @param row The PositionRow to convert.
     * @return The created Position.
     */
    finance::Position PositionFactory::fromPositionRow(const PositionRow& row)
    {
        auto position =
            finance::Position{row.createdAt.value(), row.closedAt.value()};

        position.setId(row.id.value());

        return position;
    }

    /**
     * @brief Create a vector of Positions from a vector of PositionRows.
     *
     * @param rows The vector of PositionRows to convert.
     * @return The created vector of Positions.
     */
    std::vector<finance::Position> PositionFactory::fromPositionRows(
        const std::vector<PositionRow>& rows
    )
    {
        std::vector<finance::Position> positions;
        positions.reserve(rows.size());

        for (const auto& row : rows)
            positions.push_back(fromPositionRow(row));

        return positions;
    }

    /**
     * @brief Convert a Position to a PositionRow.
     *
     * @param position The Position to convert.
     * @return The created PositionRow.
     */
    PositionRow PositionFactory::toPositionRow(
        const finance::Position& position
    )
    {
        PositionRow row;
        row.id        = position.getId();
        row.createdAt = position.getCreatedAt();
        row.closedAt  = position.getClosedAt();
        return row;
    }
}   // namespace app

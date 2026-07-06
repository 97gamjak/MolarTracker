#include "position_factory.hpp"

#include <optional>

#include "finance/position.hpp"
#include "sql_models/position_row.hpp"
#include "utils/timestamp.hpp"

namespace repo
{
    /**
     * @brief Create a Position from a PositionRow.
     *
     * @param row The PositionRow to convert.
     * @return The created Position.
     */
    finance::Position PositionFactory::fromPositionRow(
        const PositionRow& row,
        AccountId          accountId
    )
    {
        auto position = finance::Position{
            accountId,
            row.createdAt.value(),
            row.closedAt.value()
        };

        position.setId(row.id.value());

        return position;
    }

    /**
     * @brief Create a vector of Positions from a vector of PositionRows.
     *
     * @param rows The vector of pairs of PositionRows and AccountIds to
     * convert.
     * @return The created vector of Positions.
     */
    std::vector<finance::Position> PositionFactory::fromPositionRows(
        const std::vector<std::pair<PositionRow, AccountId>>& rows
    )
    {
        std::vector<finance::Position> positions;
        positions.reserve(rows.size());

        for (const auto& [row, accountId] : rows)
            positions.push_back(fromPositionRow(row, accountId));

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
}   // namespace repo

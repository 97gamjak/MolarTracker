#include "position_repo.hpp"

#include <stdexcept>

#include "app/factories/position_factory.hpp"
#include "finance/position.hpp"
#include "orm/query_options.hpp"
#include "sql_models/position_row.hpp"
#include "sql_models/trade_leg_row.hpp"

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
     * @brief Create a Position Joins object
     *
     * @return orm::Joins
     */
    orm::Joins PositionRepo::_createPositionJoins()
    {
        return std::move(
            orm::Joins{}.add(
                orm::join<PositionRow::idField, TradeLegRow::positionIdField>()
            )
        );
    }

    /**
     * @brief Create a Position Query object
     *
     * @param accountIds
     * @return orm::Query
     */
    orm::Query PositionRepo::_createPositionQuery(
        const idSet<AccountId>& accountIds
    )
    {
        return std::move(
            orm::Query{}.in<TradeLegRow::accountIdField>(accountIds)
        );
    }

    /**
     * @brief Get all Positions
     *
     * @param accountIds The IDs of the accounts to retrieve positions for.
     *
     * @return std::vector<finance::Position>
     */
    std::vector<finance::Position> PositionRepo::getAllPositions(
        const idSet<AccountId>& accountIds
    )
    {
        const auto joins = _createPositionJoins();
        const auto query = _createPositionQuery(accountIds);

        auto result = _getCrud().getJoined<PositionRow>(_getDb(), joins, query);

        std::vector<PositionRow> positionRows;
        positionRows.reserve(result.size());
        for (const auto& [row] : result)
            positionRows.push_back(row);

        return PositionFactory::fromPositionRows(positionRows);
    }

    /**
     * @brief Get all open Positions
     *
     * @param accountIds The IDs of the accounts to retrieve positions for.
     *
     * @return std::vector<finance::Position>
     */
    std::vector<finance::Position> PositionRepo::getAllOpenPositions(
        const idSet<AccountId>& accountIds
    )
    {
        const auto joins = _createPositionJoins();
        const auto query =
            _createPositionQuery(accountIds).where(PositionRow::IsOpen());

        auto result = _getCrud().getJoined<PositionRow>(_getDb(), joins, query);

        std::vector<PositionRow> positionRows;
        positionRows.reserve(result.size());
        for (const auto& [row] : result)
            positionRows.push_back(row);

        return PositionFactory::fromPositionRows(positionRows);
    }

}   // namespace app

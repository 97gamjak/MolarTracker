#include "position_repo.hpp"

#include <stdexcept>

#include "finance/position.hpp"
#include "orm/query_options.hpp"
#include "repo/factories/position_factory.hpp"
#include "sql_models/position_row.hpp"
#include "sql_models/trade_leg_row.hpp"

namespace repo
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
        return orm::Joins{}
            .add(
                orm::join<PositionRow::idField, TradeLegRow::positionIdField>()
            )
            .distinct();
    }

    /**
     * @brief Create a Position Query object
     *
     * @param accountIds
     * @return orm::Query
     */
    orm::Query PositionRepo::_createPositionQuery(
        const IdSet<AccountId>& accountIds
    )
    {
        return orm::Query{}.in<TradeLegRow::accountIdField>(accountIds);
    }

    /**
     * @brief Get a Position by its ID
     *
     * @param positionId The ID of the position to retrieve
     * @return std::optional<finance::Position>
     */
    std::optional<finance::Position> PositionRepo::getPosition(
        PositionId positionId
    )
    {
        const auto joins = _createPositionJoins();
        const auto query = orm::Query{}.where(PositionRow::hasId(positionId));

        auto result = _getCrud().getJoined<PositionRow, TradeLegRow>(
            _getDb(),
            joins,
            query
        );

        if (result.size() > 1)
        {
            throw std::runtime_error(
                std::format(
                    "Multiple positions found for position ID: {}",
                    positionId.toString()
                )
            );
        }

        if (result.empty())
            return std::nullopt;

        const auto& [positionRow, tradeLegRow] = result.front();

        return PositionFactory::fromPositionRow(
            positionRow,
            tradeLegRow.accountId.value()
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
        const IdSet<AccountId>& accountIds
    )
    {
        if (accountIds.empty())
            return {};

        const auto joins = _createPositionJoins();
        const auto query = _createPositionQuery(accountIds);

        auto result = _getCrud().getJoined<PositionRow, TradeLegRow>(
            _getDb(),
            joins,
            query
        );

        std::vector<std::pair<PositionRow, AccountId>> positionRows;
        positionRows.reserve(result.size());
        for (const auto& [row, leg] : result)
            positionRows.emplace_back(row, leg.accountId.value());

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
        const IdSet<AccountId>& accountIds
    )
    {
        if (accountIds.empty())
            return {};

        const auto joins = _createPositionJoins();
        const auto query =
            _createPositionQuery(accountIds).where(PositionRow::IsOpen());

        auto result = _getCrud().getJoined<PositionRow, TradeLegRow>(
            _getDb(),
            joins,
            query
        );

        std::vector<std::pair<PositionRow, AccountId>> positionRows;
        positionRows.reserve(result.size());
        for (const auto& [row, leg] : result)
            positionRows.emplace_back(row, leg.accountId.value());

        return PositionFactory::fromPositionRows(positionRows);
    }

}   // namespace repo

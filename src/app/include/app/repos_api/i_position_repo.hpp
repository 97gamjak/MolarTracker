#ifndef __APP__INCLUDE__APP__REPOS_API__I_POSITION_REPO_HPP__
#define __APP__INCLUDE__APP__REPOS_API__I_POSITION_REPO_HPP__

#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    class Position;   // Forward declaration
}   // namespace finance

namespace app
{
    /**
     * @brief Interface for Position repository.
     *
     */
    class IPositionRepo
    {
       public:
        virtual ~IPositionRepo() = default;

        /**
         * @brief Create a Position
         *
         * @param position
         *
         * @return PositionId
         */
        virtual PositionId createPosition(
            const finance::Position& position
        ) = 0;

        /**
         * @brief Get all Positions
         *
         * @param accountIds The IDs of the accounts to retrieve positions for.
         *
         * @return std::vector<finance::Position>
         */
        [[nodiscard]]
        virtual std::vector<finance::Position> getAllPositions(
            const idSet<AccountId>& accountIds
        ) = 0;

        /**
         * @brief Get all open Positions
         *
         * @param accountIds The IDs of the accounts to retrieve positions for.
         *
         * @return std::vector<finance::Position>
         */
        [[nodiscard]]
        virtual std::vector<finance::Position> getAllOpenPositions(
            const idSet<AccountId>& accountIds
        ) = 0;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__REPOS_API__I_POSITION_REPO_HPP__
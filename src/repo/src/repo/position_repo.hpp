#ifndef __REPO__SRC__REPO__POSITION_REPO_HPP__
#define __REPO__SRC__REPO__POSITION_REPO_HPP__

#include "base_repo.hpp"
#include "config/id_types.hpp"
#include "repo/i_position_repo.hpp"

namespace orm
{
    class Joins;   // Forward declaration
    class Query;   // Forward declaration
}   // namespace orm

namespace repo
{
    /**
     * @brief Repository for managing Positions
     *
     */
    class PositionRepo : public IPositionRepo, public BaseRepo
    {
       public:
        using BaseRepo::BaseRepo;

        [[nodiscard]]
        PositionId createPosition(const finance::Position& position) override;

        [[nodiscard]]
        std::vector<finance::Position> getAllPositions(
            const idSet<AccountId>& accountIds
        ) override;

        [[nodiscard]]
        std::vector<finance::Position> getAllOpenPositions(
            const idSet<AccountId>& accountIds
        ) override;

       private:
        [[nodiscard]]
        static orm::Joins _createPositionJoins();

        [[nodiscard]]
        static orm::Query _createPositionQuery(
            const idSet<AccountId>& accountIds
        );
    };
}   // namespace repo

#endif   // __REPO__SRC__REPO__POSITION_REPO_HPP__

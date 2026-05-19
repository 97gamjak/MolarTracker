#ifndef __APP__SRC__APP__REPOS__POSITION_REPO_HPP__
#define __APP__SRC__APP__REPOS__POSITION_REPO_HPP__

#include "app/repos/base_repo.hpp"
#include "app/repos_api/i_position_repo.hpp"
#include "config/id_types.hpp"

namespace app
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
        std::vector<finance::Position> getAllPositions() override;

        [[nodiscard]]
        std::vector<finance::Position> getAllOpenPositions() override;
    };
}   // namespace app

#endif   // __APP__SRC__APP__REPOS__POSITION_REPO_HPP__

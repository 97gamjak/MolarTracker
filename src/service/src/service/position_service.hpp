#ifndef __SERVICE__SRC__SERVICE__POSITION_SERVICE_HPP__
#define __SERVICE__SRC__SERVICE__POSITION_SERVICE_HPP__

#include <memory>

#include "service/i_position_service.hpp"

namespace repo
{
    class IPositionRepo;   // Forward declaration
}   // namespace repo

namespace service
{
    /**
     * @brief Position Service Implementation
     *
     */
    class PositionService : public IPositionService
    {
       private:
        /// reference to the position repository
        std::shared_ptr<repo::IPositionRepo> _positionRepo;

       public:
        explicit PositionService(
            std::shared_ptr<repo::IPositionRepo> positionRepo
        );

        [[nodiscard]]
        PositionId createPosition(const finance::Position& position) override;

        [[nodiscard]]
        std::vector<finance::Position> getAllPositions(
            const IdSet<AccountId>& accountIds
        ) override;

        [[nodiscard]]
        std::vector<finance::Position> getAllOpenPositions(
            const IdSet<AccountId>& accountIds
        ) override;
    };

}   // namespace service

#endif   // __SERVICE__SRC__SERVICE__POSITION_SERVICE_HPP__
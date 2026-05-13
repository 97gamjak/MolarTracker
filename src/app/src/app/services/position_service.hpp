#ifndef __APP__SRC__APP__SERVICES__POSITION_SERVICE_HPP__
#define __APP__SRC__APP__SERVICES__POSITION_SERVICE_HPP__

#include <memory>

#include "app/services_api/i_position_service.hpp"

namespace app
{
    class IPositionRepo;   // Forward declaration

    /**
     * @brief Position Service Implementation
     *
     */
    class PositionService : public IPositionService
    {
       private:
        /// reference to the position repository
        std::shared_ptr<IPositionRepo> _positionRepo;

       public:
        explicit PositionService(std::shared_ptr<IPositionRepo> positionRepo);

        [[nodiscard]]
        PositionId createPosition(const finance::Position& position) override;

        [[nodiscard]]
        std::vector<finance::Position> getAllPositions() override;

        [[nodiscard]]
        std::vector<finance::Position> getAllOpenPositions() override;
    };

}   // namespace app

#endif   // __APP__SRC__APP__SERVICES__POSITION_SERVICE_HPP__
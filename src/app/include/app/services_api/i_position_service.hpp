#ifndef __APP__INCLUDE__APP__SERVICES_API__I_POSITION_SERVICE_HPP__
#define __APP__INCLUDE__APP__SERVICES_API__I_POSITION_SERVICE_HPP__

#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    class Position;   // Forward declaration
}   // namespace finance

namespace app
{

    /**
     * @brief Interface for Position Service
     *
     */
    class IPositionService
    {
       public:
        virtual ~IPositionService() = default;

        /**
         * @brief Create a new Position
         *
         * @param position
         * @return PositionId
         */
        [[nodiscard]]
        virtual PositionId createPosition(
            const finance::Position& position
        ) = 0;

        /**
         * @brief Get all Positions
         *
         * @return std::vector<finance::Position>
         */
        [[nodiscard]]
        virtual std::vector<finance::Position> getAllPositions() = 0;

        /**
         * @brief Get all open Positions
         *
         * @return std::vector<finance::Position>
         */
        [[nodiscard]]
        virtual std::vector<finance::Position> getAllOpenPositions() = 0;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__SERVICES_API__I_POSITION_SERVICE_HPP__
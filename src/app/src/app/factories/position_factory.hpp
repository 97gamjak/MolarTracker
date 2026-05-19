#ifndef __APP__SRC__APP__FACTORIES__POSITION_FACTORY_HPP__
#define __APP__SRC__APP__FACTORIES__POSITION_FACTORY_HPP__

#include <vector>

namespace finance
{
    class Position;   // Forward declaration
}   // namespace finance

struct PositionRow;   // Forward declaration

namespace app
{
    /**
     * @brief Factory for creating Position objects.
     *
     */
    class PositionFactory
    {
       public:
        static finance::Position fromPositionRow(const PositionRow& row);

        static std::vector<finance::Position> fromPositionRows(
            const std::vector<PositionRow>& rows
        );

        static PositionRow toPositionRow(const finance::Position& position);
    };
}   // namespace app

#endif   // __APP__SRC__APP__FACTORIES__POSITION_FACTORY_HPP__
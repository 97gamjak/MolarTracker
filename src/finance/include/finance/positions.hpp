#ifndef __FINANCE__INCLUDE__FINANCE__POSITIONS_HPP__
#define __FINANCE__INCLUDE__FINANCE__POSITIONS_HPP__

#include "position.hpp"
#include "utils/container/id_map.hpp"

namespace finance
{
    /**
     * @brief Represents a collection of positions, which are financial
     * instruments that represent ownership in a company and constitute a claim
     * on part of the company's assets and earnings. The Positions class extends
     * an IdObjectMap of Position and provides methods to access individual
     * positions by their position ID, allowing for efficient management and
     * retrieval of position information in a financial context.
     *
     */
    class Positions : public IdObjectMap<Position>
    {
       public:
        [[nodiscard]]
        Positions getOpenPositions() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__POSITIONS_HPP__
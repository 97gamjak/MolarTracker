#ifndef __FINANCE__INCLUDE__FINANCE__POSITIONS_HPP__
#define __FINANCE__INCLUDE__FINANCE__POSITIONS_HPP__

#include "common/container/id_map.hpp"
#include "position.hpp"

namespace finance
{
    class Positions : public IdObjectMap<Position>
    {
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__POSITIONS_HPP__
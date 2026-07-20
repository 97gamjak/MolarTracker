#include "finance/positions.hpp"

namespace finance
{
    Positions Positions::getOpenPositions() const
    {
        Positions openPositions;

        for (const auto& [id, position] : *this)
        {
            if (position.isOpen())
            {
                openPositions.addUnchecked(position);
            }
        }

        return openPositions;
    }
}   // namespace finance
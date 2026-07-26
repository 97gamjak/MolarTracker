#include "finance/positions.hpp"

namespace finance
{
    /**
     * @brief Get the open positions from the collection of positions, this will
     * filter through the positions and return a new Positions object containing
     * only the positions that are currently open (i.e., not closed or fully
     * liquidated).
     *
     * @return Positions The collection of open positions.
     */
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
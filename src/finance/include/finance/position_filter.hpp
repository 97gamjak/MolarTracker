#ifndef __FINANCE__INCLUDE__FINANCE__POSITION_FILTER_HPP__
#define __FINANCE__INCLUDE__FINANCE__POSITION_FILTER_HPP__

#include <optional>

#include "config/id_types.hpp"
#include "utils/container/set.hpp"

namespace finance
{
    /**
     * @brief Filter for positions, used to filter positions based on their
     * status (open or closed).
     *
     */
    struct PositionFilter
    {
        IdSet<AccountId> accountIds;

        /// The set of position IDs to filter by, if specified
        std::optional<bool> isOpen = std::nullopt;

        explicit PositionFilter(const IdSet<AccountId>& accountIds);
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__POSITION_FILTER_HPP__
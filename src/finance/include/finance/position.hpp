#ifndef __FINANCE__INCLUDE__FINANCE__POSITION_HPP__
#define __FINANCE__INCLUDE__FINANCE__POSITION_HPP__

#include <optional>

#include "config/id_types.hpp"
#include "filter/predicate.hpp"
#include "utils/container/set.hpp"
#include "utils/timestamp.hpp"

namespace finance
{

    /**
     * @brief Represents a financial position.
     *
     */
    class Position
    {
       private:
        /// Position ID
        PositionId _id;

        /// Account ID associated with the position
        AccountId _accountId;

        /// Creation timestamp
        Timestamp _createdAt;
        /// Closing timestamp
        std::optional<Timestamp> _closedAt;

       public:
        explicit Position(
            PositionId               positionId,
            AccountId                accountId,
            Timestamp                createdAt,
            std::optional<Timestamp> closedAt = std::nullopt
        );

        void setId(PositionId id);

        [[nodiscard]] PositionId               getId() const;
        [[nodiscard]] AccountId                getAccountId() const;
        [[nodiscard]] Timestamp                getCreatedAt() const;
        [[nodiscard]] std::optional<Timestamp> getClosedAt() const;

        [[nodiscard]] std::string toString() const;
    };

    filter::Predicate<Position> IsPositionOpen(bool isOpen = true);

    filter::Predicate<Position> IsPositionForAccounts(
        const IdSet<AccountId>& accountIds
    );

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__POSITION_HPP__
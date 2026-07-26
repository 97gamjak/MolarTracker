#ifndef __FINANCE__INCLUDE__FINANCE__POSITION_HPP__
#define __FINANCE__INCLUDE__FINANCE__POSITION_HPP__

#include <optional>

#include "common/timestamp.hpp"
#include "config/id_types.hpp"
#include "filter/predicate.hpp"

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

        /// Creation timestamp
        Timestamp _createdAt;
        /// Closing timestamp
        std::optional<Timestamp> _closedAt;

       public:
        explicit Position(
            Timestamp                createdAt,
            std::optional<Timestamp> closedAt = std::nullopt
        );

        void setId(PositionId id);

        [[nodiscard]] PositionId               getId() const;
        [[nodiscard]] Timestamp                getCreatedAt() const;
        [[nodiscard]] std::optional<Timestamp> getClosedAt() const;

        [[nodiscard]] bool isOpen() const;

        [[nodiscard]] std::string toString() const;
    };

    filter::Predicate<Position> IsPositionOpen(bool isOpen = true);

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__POSITION_HPP__
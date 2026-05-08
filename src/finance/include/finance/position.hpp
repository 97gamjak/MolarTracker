#ifndef __FINANCE__INCLUDE__FINANCE__POSITION_HPP__
#define __FINANCE__INCLUDE__FINANCE__POSITION_HPP__

#include "config/id_types.hpp"
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

        /// Creation timestamp
        Timestamp _createdAt;
        /// Closing timestamp
        std::optional<Timestamp> _closedAt;

       public:
        Position(Timestamp createdAt, std::optional<Timestamp> closedAt);

        void setId(PositionId id);

        [[nodiscard]] PositionId               getId() const;
        [[nodiscard]] Timestamp                getCreatedAt() const;
        [[nodiscard]] std::optional<Timestamp> getClosedAt() const;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__POSITION_HPP__
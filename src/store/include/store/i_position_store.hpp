#ifndef __STORE__INCLUDE__STORE__I_POSITION_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_POSITION_STORE_HPP__

#include "config/id_types.hpp"
#include "finance/positions.hpp"   // to avoid incomplete return type outside

namespace finance
{
    class Position;         // Forward declaration
    class AccountsView;     // Forward declaration
    class PositionFilter;   // Forward declaration
}   // namespace finance

class Connection;   // Forward declaration

namespace store
{

    class IPositionStoreReader
    {
       protected:
        /**
         * @brief Structure representing a callback for when a position is
         * closed.
         *
         */
        struct PositionClosed
        {
            /// The callback function type for when a position is closed
            using func = std::function<void(PositionId)>;
        };

       public:
        virtual ~IPositionStoreReader() = default;

        [[nodiscard]]
        virtual finance::Positions getPositions(
            const finance::PositionFilter& filter
        ) const = 0;

        /**
         * @brief Get a Position by its ID
         *
         * @param positionId The ID of the position to retrieve
         *
         * @return std::optional<finance::Position>
         */
        [[nodiscard]]
        virtual std::optional<finance::Position> getPosition(
            PositionId positionId
        ) const = 0;

        /**
         * @brief Subscribe to position closed events, this allows subscribers
         * to be notified when a position is closed, which can be useful for
         * updating the UI or performing other actions in response to a position
         * being closed.
         *
         * @param func
         * @param user
         * @return Connection
         */
        [[nodiscard]]
        virtual Connection subscribeToPositionClosed(
            PositionClosed::func func,
            void*                user
        ) = 0;
    };

    /**
     * @brief Store for managing Positions
     *
     */
    class IPositionStore
    {
       public:
        virtual ~IPositionStore() = default;

        /**
         * @brief Create a Position
         *
         * @param position
         * @return PositionId
         */
        [[nodiscard]]
        virtual PositionId createPosition(
            const finance::Position& position
        ) = 0;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_POSITION_STORE_HPP__
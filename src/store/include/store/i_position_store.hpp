#ifndef __STORE__INCLUDE__STORE__I_POSITION_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_POSITION_STORE_HPP__

#include "config/id_types.hpp"
#include "finance/positions.hpp"   // to avoid incomplete return type outside

namespace finance
{
    class Position;   // Forward declaration
}   // namespace finance

class Connection;   // Forward declaration

namespace store
{

    struct PositionClosed
    {
        using func = std::function<void(PositionId)>;
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

        /**
         * @brief Get all Positions
         *
         * @return finance::Positions
         */
        [[nodiscard]]
        virtual finance::Positions getAllPositions() const = 0;

        /**
         * @brief Get all open Positions
         *
         * @return finance::Positions
         */
        [[nodiscard]]
        virtual finance::Positions getOpenPositions() const = 0;

        /**
         * @brief Commit the current session
         *
         */
        virtual void commit() = 0;

        /**
         * @brief Get the ID remapping for positions
         *
         * @return const unorderedIdMap<PositionId, PositionId>&
         */
        [[nodiscard]]
        virtual const unorderedIdMap<PositionId, PositionId>& getIdRemap(
        ) const = 0;

        [[nodiscard]]
        virtual Connection subscribeToPositionClosed(
            PositionClosed::func func,
            void*                user
        ) = 0;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_POSITION_STORE_HPP__
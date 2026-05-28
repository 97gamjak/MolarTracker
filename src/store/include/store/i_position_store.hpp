#ifndef __STORE__INCLUDE__STORE__I_POSITION_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_POSITION_STORE_HPP__

#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    class Position;   // Forward declaration
}   // namespace finance

namespace store
{
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
         * @return std::vector<finance::Position>
         */
        [[nodiscard]]
        virtual std::vector<finance::Position> getAllPositions() const = 0;

        /**
         * @brief Get all open Positions
         *
         * @return std::vector<finance::Position>
         */
        [[nodiscard]]
        virtual std::vector<finance::Position> getOpenPositions() const = 0;

        /**
         * @brief Get all open Positions for specific accounts
         *
         * @param accountIds
         * @return std::vector<finance::Position>
         */
        [[nodiscard]]
        virtual std::vector<finance::Position> getOpenPositions(
            const idSet<AccountId>& accountIds
        ) const = 0;

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
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_POSITION_STORE_HPP__
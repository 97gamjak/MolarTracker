#ifndef __APP__INCLUDE__APP__STORE__POSITION_STORE_HPP__
#define __APP__INCLUDE__APP__STORE__POSITION_STORE_HPP__

#include <vector>

#include "app/services_api/i_position_service.hpp"
#include "base/base_store.hpp"
#include "finance/position.hpp"

namespace app
{
    class TransactionStore;   // Forward declaration

    /**
     * @brief Store for managing Positions
     *
     */
    class PositionStore : public BaseStore<finance::Position, PositionId>
    {
       private:
        /// The Position service
        std::shared_ptr<IPositionService> _positionService;

       public:
        explicit PositionStore(
            std::shared_ptr<IPositionService> positionService
        );

        [[nodiscard]]
        PositionId createPosition(const finance::Position& position);

        [[nodiscard]] std::vector<finance::Position> getAllPositions() const;
        [[nodiscard]] std::vector<finance::Position> getOpenPositions() const;

        void commit();
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__POSITION_STORE_HPP__
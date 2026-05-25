#ifndef __STORE__INCLUDE__STORE__POSITION_STORE_HPP__
#define __STORE__INCLUDE__STORE__POSITION_STORE_HPP__

#include <vector>

#include "base/base_store.hpp"
#include "finance/position.hpp"
#include "service/i_position_service.hpp"

namespace store
{
    class TransactionStore;   // Forward declaration
    class AccountSession;     // Forward declaration

    /**
     * @brief Store for managing Positions
     *
     */
    class PositionStore : public BaseStore<finance::Position, PositionId>
    {
       private:
        /// The Position service
        std::shared_ptr<service::IPositionService> _positionService;

        struct Session;
        /// The current session data
        std::unique_ptr<Session> _session;

       public:
        explicit PositionStore(
            std::shared_ptr<service::IPositionService> positionService,
            const AccountSession&                      accountSession
        );

        ~PositionStore() override;

        [[nodiscard]]
        PositionId createPosition(const finance::Position& position);

        [[nodiscard]] std::vector<finance::Position> getAllPositions() const;
        [[nodiscard]] std::vector<finance::Position> getOpenPositions() const;

        void commit();
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__POSITION_STORE_HPP__
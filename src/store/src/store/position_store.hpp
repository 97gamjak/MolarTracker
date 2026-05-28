#ifndef __STORE__SRC__STORE__POSITION_STORE_HPP__
#define __STORE__SRC__STORE__POSITION_STORE_HPP__

#include <vector>

#include "base/base_store.hpp"
#include "config/id_types.hpp"
#include "config/strong_id.hpp"
#include "finance/position.hpp"
#include "service/i_position_service.hpp"
#include "store/i_position_store.hpp"

namespace store
{
    class TransactionStore;   // Forward declaration
    class AccountSession;     // Forward declaration

    /**
     * @brief Store for managing Positions
     *
     */
    class PositionStore : public BaseStore<finance::Position, PositionId>,
                          public IPositionStore
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
        PositionId createPosition(const finance::Position& position) override;

        [[nodiscard]]
        std::vector<finance::Position> getAllPositions() const override;

        [[nodiscard]]
        std::vector<finance::Position> getOpenPositions() const override;

        void commit() override;

        [[nodiscard]]
        const unorderedIdMap<PositionId, PositionId>& getIdRemap(
        ) const override;
    };

}   // namespace store

#endif   // __STORE__SRC__STORE__POSITION_STORE_HPP__
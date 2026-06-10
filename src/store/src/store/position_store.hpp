#ifndef __STORE__SRC__STORE__POSITION_STORE_HPP__
#define __STORE__SRC__STORE__POSITION_STORE_HPP__

#include "base/base_store.hpp"
#include "config/id_types.hpp"
#include "config/strong_id.hpp"
#include "finance/position.hpp"
#include "service/i_position_service.hpp"
#include "store/i_position_store.hpp"

namespace finance
{
    class Accounts;   // Forward declaration
}   // namespace finance

namespace store
{
    class TransactionStore;   // Forward declaration

    /**
     * @brief Store for managing Positions
     *
     */
    struct PositionFilter
    {
        std::optional<bool> isOpen = std::nullopt;
    };

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

        std::unique_ptr<Observable<PositionClosed>> _positionEvents;

       public:
        explicit PositionStore(
            std::shared_ptr<service::IPositionService> positionService,
            const finance::Accounts&                   accountSession
        );

        ~PositionStore() override;

        [[nodiscard]]
        PositionId createPosition(const finance::Position& position) override;

        [[nodiscard]]
        finance::Positions getOpenPositions() const override;

        [[nodiscard]]
        finance::Positions getAllPositions() const override;

        void commit() override;

        [[nodiscard]]
        const unorderedIdMap<PositionId, PositionId>& getIdRemap(
        ) const override;

        [[nodiscard]]
        Connection subscribeToPositionClosed(
            PositionClosed::func func,
            void*                user
        ) override;

       private:
        finance::Positions _getPositions(PositionFilter filter) const;
    };

}   // namespace store

#endif   // __STORE__SRC__STORE__POSITION_STORE_HPP__
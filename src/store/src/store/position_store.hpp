#ifndef __STORE__SRC__STORE__POSITION_STORE_HPP__
#define __STORE__SRC__STORE__POSITION_STORE_HPP__

#include "base/base_store.hpp"
#include "config/id_types.hpp"
#include "finance/position.hpp"
#include "service/i_position_service.hpp"
#include "store/i_position_store.hpp"

namespace finance
{
    class AccountsView;
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
        /// The set of position IDs to filter by, if specified
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

        /// The position events observable
        std::unique_ptr<Observable<PositionClosed>> _positionEvents;

       public:
        explicit PositionStore(
            std::shared_ptr<service::IPositionService> positionService
        );

        ~PositionStore() override;

        [[nodiscard]]
        PositionId createPosition(const finance::Position& position) override;

        [[nodiscard]]
        finance::Positions getOpenPositions(
            const finance::AccountsView& accounts
        ) const override;

        [[nodiscard]]
        finance::Positions getAllPositions(
            const finance::AccountsView& accounts
        ) const override;

        void commit();

        [[nodiscard]]
        Connection subscribeToPositionClosed(
            PositionClosed::func func,
            void*                user
        ) override;
    };

}   // namespace store

#endif   // __STORE__SRC__STORE__POSITION_STORE_HPP__
#ifndef __GATEWAY__INCLUDE__GATEWAY__POSITION_GATEWAY_HPP__
#define __GATEWAY__INCLUDE__GATEWAY__POSITION_GATEWAY_HPP__

#include <memory>

#include "finance/transaction/pnl.hpp"
#include "finance/transaction/transactions.hpp"   // for return value

namespace store
{
    class ITransactionStore;   // forward declaration
    class IPositionStore;      // forward declaration
    class IOptionStore;        // forward declaration
}   // namespace store

namespace gateway
{
    class PositionGateway
    {
       private:
        std::shared_ptr<store::ITransactionStore> _transactionStore;
        std::shared_ptr<store::IPositionStore>    _positionStore;
        std::shared_ptr<store::IOptionStore>      _optionStore;

       public:
        PositionGateway(
            const std::shared_ptr<store::ITransactionStore>& transactionStore,
            const std::shared_ptr<store::IPositionStore>&    positionStore,
            const std::shared_ptr<store::IOptionStore>&      optionStore
        );

        [[nodiscard]]
        FinanceResult<IdMap<PositionId, finance::Transactions>> getOpenPositionTransactions(
            const IdSet<AccountId>& accountIds
        ) const;

        [[nodiscard]]
        FinanceResult<finance::PositionEvents> getPositionEvents(
            const finance::Transactions&                txs,
            const std::shared_ptr<store::IOptionStore>& optionStore
        ) const;
    };
}   // namespace gateway

#endif   // __GATEWAY__INCLUDE__GATEWAY__POSITION_GATEWAY_HPP__
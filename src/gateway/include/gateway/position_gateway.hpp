#ifndef __GATEWAY__INCLUDE__GATEWAY__POSITION_GATEWAY_HPP__
#define __GATEWAY__INCLUDE__GATEWAY__POSITION_GATEWAY_HPP__

#include <memory>

#include "finance/transaction/position_transaction.hpp"
#include "finance/transaction/transactions.hpp"

namespace store
{
    class ITransactionStore;   // forward declaration
    class IPositionStore;      // forward declaration
    class IOptionStore;        // forward declaration
}   // namespace store

namespace finance
{
    class TransactionFilter;   // forward declaration
    class Options;             // forward declaration
    class Positions;           // forward declaration
}   // namespace finance

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
        FinanceResult<finance::PositionTransactions> getOpenPositionTransactions(
            const IdSet<AccountId>& accountIds
        ) const;

       private:
        [[nodiscard]]
        std::pair<
            finance::Positions,
            finance::TransactionFilter> _getOpenPositionsFilter() const;

        [[nodiscard]]
        finance::Options _getNeededOptions(
            const finance::Transactions& txs
        ) const;
    };
}   // namespace gateway

#endif   // __GATEWAY__INCLUDE__GATEWAY__POSITION_GATEWAY_HPP__
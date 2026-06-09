#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__POSITION_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__POSITION_TRANSACTION_HPP__

#include <memory>

#include "config/id_types.hpp"
#include "pnl.hpp"
#include "transactions.hpp"

namespace finance
{
    class StockPositionTransaction : protected StockTransactions
    {
        PositionId _positionId = PositionId::invalid();

        InstrumentId _baseInstrument  = InstrumentId::invalid();
        AccountId    _securityAccount = AccountId::invalid();

        std::unique_ptr<PnL> _pnl;
        bool                 _pnlReady = false;

       public:
        explicit StockPositionTransaction() = default;
        explicit StockPositionTransaction(PositionId id);

        [[nodiscard]] bool add(const StockTransaction& txs);

        [[nodiscard]] InstrumentId getBaseInstrument() const;
        [[nodiscard]] AccountId    getSecurityAccount() const;
        [[nodiscard]] PnL*         getPnL();

        void sort();

        using StockTransactions::empty;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__POSITION_TRANSACTION_HPP__

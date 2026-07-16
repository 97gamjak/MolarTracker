#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__POSITION_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__POSITION_TRANSACTION_HPP__

#include <memory>

#include "config/id_types.hpp"
#include "pnl.hpp"
#include "transactions.hpp"

namespace finance
{
    /**
     * @brief Class representing a stock position transaction, containing all
     * transactions related to a specific position, along with PnL information.
     *
     */
    class StockPositionTransaction : protected StockTransactions
    {
        /// The ID of the position associated with the stock transactions
        PositionId _positionId = PositionId::invalid();
        /// The base instrument associated with the position
        InstrumentId _baseInstrument = InstrumentId::invalid();
        /// The security account associated with the position
        AccountId _securityAccount = AccountId::invalid();

        /// The PnL information for the position, this will be calculated based
        /// on the transactions in the position and the current price of the
        /// security
        std::shared_ptr<PnL> _pnl;
        /// Flag indicating whether the PnL information is up to date and ready
        /// for display
        bool _pnlReady = false;

        /// to hide the add method from StockTransactions and ensure that only
        /// transactions that match the base instrument and security account can
        /// be added to the position, this will help maintain the integrity of
        /// the position and ensure that all transactions in the position are
        /// related to the same security and account.
        using StockTransactions::add;

       public:
        explicit StockPositionTransaction() = default;
        explicit StockPositionTransaction(PositionId id);

        [[nodiscard]] bool addPosition(const StockTransaction& txs);

        [[nodiscard]] InstrumentId                getBaseInstrument() const;
        [[nodiscard]] AccountId                   getSecurityAccount() const;
        [[nodiscard]] const std::shared_ptr<PnL>& getPnL();

        using StockTransactions::empty;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__POSITION_TRANSACTION_HPP__

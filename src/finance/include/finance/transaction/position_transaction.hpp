#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__POSITION_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__POSITION_TRANSACTION_HPP__

#include <memory>

#include "config/id_types.hpp"
#include "error/finance_error.hpp"
#include "finance/position.hpp"
#include "pnl.hpp"
#include "transactions.hpp"
#include "utils/finance.hpp"

namespace finance
{
    class Positions;

    /**
     * @brief Class representing a stock position transaction, containing all
     * transactions related to a specific position, along with PnL information.
     *
     */
    class PositionTransaction
    {
        Position _position;
        /// The base instrument associated with the position
        InstrumentId _baseInstrument;
        /// The security account associated with the position
        AccountId _securityAccount;

        InstrumentType _instrumentType;

        Transactions _transactions;

        /// The PnL information for the position, this will be calculated based
        /// on the transactions in the position and the current price of the
        /// security
        std::shared_ptr<PnL> _pnl;
        /// Flag indicating whether the PnL information is up to date and ready
        /// for display
        bool _pnlReady = false;

       public:
        explicit PositionTransaction(
            const Position&     position,
            InstrumentId        baseInstrument,
            AccountId           securityAccount,
            InstrumentType      instrumentType,
            const Transactions& transactions
        );

        [[nodiscard]] const Transactions& getTransactions() const;

        [[nodiscard]] PositionId      getId() const;
        [[nodiscard]] InstrumentType  getInstrumentType() const;
        [[nodiscard]] InstrumentId    getBaseInstrument() const;
        [[nodiscard]] AccountId       getSecurityAccount() const;
        [[nodiscard]] const Position& getPosition() const;

        [[nodiscard]] PnLResult<const std::shared_ptr<PnL>&> getPnL();

        [[nodiscard]]
        static PositionTransaction fromTransactions(
            const Position&     position,
            InstrumentType      instrumentType,
            const Transactions& transactions
        );
    };

    class PositionTransactions
    {
        std::vector<PositionTransaction> _stockPositions;
        std::vector<PositionTransaction> _optionPositions;

       public:
        [[nodiscard]]
        static PositionTransactions fromTransactions(
            const Transactions& transactions,
            const Positions&    positions
        );

        [[nodiscard]] std::vector<PositionTransaction> getStockPositions(
        ) const;
        [[nodiscard]]
        std::vector<PositionTransaction> getOptionPositions() const;
        [[nodiscard]]
        std::vector<PositionTransaction> getAllPositions() const;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__POSITION_TRANSACTION_HPP__

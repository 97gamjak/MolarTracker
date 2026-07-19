#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__POSITION_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__POSITION_TRANSACTION_HPP__

#include <memory>

#include "config/id_types.hpp"
#include "transactions.hpp"
#include "utils/finance.hpp"

namespace finance
{
    class Positions;
    class Option;

    struct PositionTransaction
    {
        PositionId     positionId;
        InstrumentId   baseInstrument;
        AccountId      securityAccount;
        InstrumentType instrumentType;
        Transactions   transactions;
    };

    class StockPositionTransaction : public PositionTransaction
    {
       public:
        using PositionTransaction::PositionTransaction;
    };

    class OptionPositionTransaction : public PositionTransaction
    {
       private:
        std::shared_ptr<Option> _option;

       public:
        using PositionTransaction::PositionTransaction;

        [[nodiscard]] std::shared_ptr<Option> getOption() const;
    };

    class StockPositionTransactions : public Vector<StockPositionTransaction>
    {
       public:
        using Vector<StockPositionTransaction>::Vector;
    };

    class OptionPositionTransactions : public Vector<OptionPositionTransaction>
    {
       public:
        using Vector<OptionPositionTransaction>::Vector;
    };

    class PositionTransactions
    {
        StockPositionTransactions  _stockPositions;
        OptionPositionTransactions _optionPositions;

       public:
        [[nodiscard]]
        static PositionTransactions fromTransactions(
            const Transactions& transactions,
            const Positions&    positions
        );

        [[nodiscard]]
        StockPositionTransactions getStockPositions() const;
        [[nodiscard]]
        OptionPositionTransactions getOptionPositions() const;
        [[nodiscard]]
        std::vector<std::shared_ptr<PositionTransaction>> getAllPositions(
        ) const;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__POSITION_TRANSACTION_HPP__

#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__

#include <vector>

#include "error/finance_error.hpp"
#include "utils/cash.hpp"
#include "utils/container/vector.hpp"
#include "utils/percentage.hpp"
#include "utils/quantity.hpp"
#include "utils/timestamp.hpp"

namespace finance
{

    struct StockTrade
    {
        Quantity quantity;
        Cash     unitPrice;
        Cash     fees;
    };

    struct OptionTrade
    {
        OptionType              type;
        OptionBuySell           buySell;
        TransactionOptionAction action;
        Cash                    strike;
        Quantity                quantity;
        std::int64_t            contractSize;
        Cash                    premium;
        Cash                    fees;
    };

    struct PositionEvent
    {
        Timestamp                             timestamp;
        std::variant<StockTrade, OptionTrade> data;
    };

    class PositionEvents : public Vector<PositionEvent>
    {
       public:
        using Vector<PositionEvent>::Vector;

        void sort();
    };

    struct OpenOptionLeg
    {
        OptionType    type;
        OptionBuySell buySell;
        Cash          strikePrice;
        Quantity      qty;
    };

    struct PositionState
    {
        Quantity                   openQuantity{0};
        Cash                       costBasis;
        Cash                       realizedPnL;
        Cash                       realizedCostBasis;
        Cash                       unrealizedOptionPnL;
        Cash                       fees;
        std::int64_t               contractSize{0};
        std::vector<OpenOptionLeg> openOptionLegs;
    };

    struct PositionPnl
    {
        Quantity quantity;
        Cash     costBasis;
        Cash     realizedPnL;
        Cash     realizedCostBasis;
        Cash     unrealizedPnL;
        Cash     fees;

        [[nodiscard]] Cash getAverageCost() const
        {
            return quantity.isZero() ? Cash{costBasis.getCurrency(), 0}
                                     : costBasis / quantity;
        }

        [[nodiscard]] Percentage getRealizedPnLPercentage() const
        {
            if (realizedCostBasis.isZero())
                return Percentage(0);
            return Percentage(realizedPnL / realizedCostBasis);
        }

        [[nodiscard]] Cash totalPnL() const
        {
            return realizedPnL + unrealizedPnL;
        }

        [[nodiscard]] Cash getMarketValue() const
        {
            return costBasis + totalPnL();
        }

        [[nodiscard]] Percentage getUnrealizedPnLPercentage() const
        {
            if (costBasis.isZero())
                return Percentage(0);
            return Percentage(unrealizedPnL / costBasis);
        }
    };

    [[nodiscard]]
    PnLResult<PositionState> foldEvents(
        PositionState                  state,
        std::span<const PositionEvent> events
    );

    [[nodiscard]]
    PositionPnl snapshot(
        const PositionState& state,
        std::optional<Cash>  markPrice
    );

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__
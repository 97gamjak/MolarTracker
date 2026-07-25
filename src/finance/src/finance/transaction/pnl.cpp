#include "finance/transaction/pnl.hpp"

#include <algorithm>

#include "error/finance_error.hpp"
#include "utils/cash.hpp"
#include "utils/finance.hpp"

namespace finance
{

    void PositionEvents::sort()
    {
        std::ranges::sort(
            *this,
            [](const auto& lhs, const auto& rhs)
            { return lhs.timestamp < rhs.timestamp; }
        );
    }

    [[nodiscard]]
    PositionState apply(PositionState state, const StockTrade& trade)
    {
        state.fees += trade.fees;

        if (trade.quantity.isZero())
            return state;

        const bool isStateQtyPositive = state.openQuantity > 0;
        const bool isTradeQtyPositive = trade.quantity > 0;
        const bool sameDirection      = state.openQuantity.isZero() ||
                                   (isStateQtyPositive == isTradeQtyPositive);

        if (sameDirection)
        {
            // opening or adding to the position
            state.costBasis    += trade.unitPrice * trade.quantity.abs();
            state.openQuantity += trade.quantity;
        }
        else
        {
            // reducing or closing/flipping the position
            const auto avgCost = state.costBasis / state.openQuantity.abs();
            const auto closedQty =
                std::min(trade.quantity.abs(), state.openQuantity.abs());

            state.realizedPnL       += (trade.unitPrice - avgCost) * closedQty;
            state.realizedCostBasis += avgCost * closedQty;
            state.costBasis         -= avgCost * closedQty;
            state.openQuantity -=
                (state.openQuantity > 0 ? closedQty : -closedQty);

            const auto remainder = trade.quantity.abs() - closedQty;
            if (!remainder.isZero())
            {
                // trade flips through zero: remainder opens a new position
                // in the opposite direction
                state.costBasis = trade.unitPrice * remainder;
                state.openQuantity =
                    trade.quantity > 0 ? remainder : -remainder;
            }
        }

        return state;
    }

    [[nodiscard]]
    PnLResult<PositionState> apply(
        PositionState      state,
        const OptionTrade& trade
    )
    {
        using enum OptionType;
        using enum OptionBuySell;
        using enum TransactionOptionAction;

        if (state.contractSize == 0)
            state.contractSize = trade.contractSize;
        else if (state.contractSize != trade.contractSize)
        {
            return PnLError{
                PnLErrorType::InconsistentContractSize,
                "Inconsistent contract size in option transactions"
            };
        }

        state.fees += trade.fees;

        switch (trade.action)
        {
            case Open:
            case RollOpen:
            {
                auto it = std::ranges::find_if(
                    state.openOptionLegs,
                    [&](const auto& leg)
                    {
                        return leg.type == trade.type &&
                               leg.buySell == trade.buySell &&
                               leg.strikePrice == trade.strike;
                    }
                );

                if (it != state.openOptionLegs.end())
                    it->qty += trade.quantity;
                else
                {
                    state.openOptionLegs.emplace_back(
                        trade.type,
                        trade.buySell,
                        trade.strike,
                        trade.quantity
                    );
                }

                if (trade.buySell == Buy)
                {
                    state.unrealizedOptionPnL -= trade.premium;
                    state.costBasis           += trade.premium;
                }
                else
                {
                    state.unrealizedOptionPnL += trade.premium;
                    state.costBasis +=
                        trade.strike * (trade.quantity * state.contractSize);
                }
                break;
            }

            case Close:
            case RollClose:
            {
                const auto openSide = (trade.buySell == Buy) ? Sell : Buy;
                auto       it       = std::ranges::find_if(
                    state.openOptionLegs,
                    [&](const auto& leg)
                    {
                        return leg.type == trade.type &&
                               leg.buySell == openSide &&
                               leg.strikePrice == trade.strike;
                    }
                );

                Cash realized;
                Cash costReduction;
                if (it != state.openOptionLegs.end() && !it->qty.isZero())
                {
                    realized =
                        state.unrealizedOptionPnL / it->qty * trade.quantity;
                    costReduction = state.costBasis / it->qty * trade.quantity;

                    it->qty -= trade.quantity;
                    if (it->qty.isZero())
                        state.openOptionLegs.erase(it);
                }

                state.unrealizedOptionPnL -= realized;
                state.costBasis           -= costReduction;
                state.realizedCostBasis   += costReduction;
                state.realizedPnL         += (trade.buySell == Buy)
                                                 ? (realized - trade.premium)
                                                 : (realized + trade.premium);
                break;
            }

            case Exercised:
            {
                const auto openSide = (trade.buySell == Buy) ? Sell : Buy;
                auto       it       = std::ranges::find_if(
                    state.openOptionLegs,
                    [&](const auto& leg)
                    {
                        return leg.type == trade.type &&
                               leg.buySell == openSide &&
                               leg.strikePrice == trade.strike;
                    }
                );

                Cash realized;
                Cash costReduction;
                if (it != state.openOptionLegs.end() && !it->qty.isZero())
                {
                    realized =
                        state.unrealizedOptionPnL / it->qty * trade.quantity;
                    costReduction = state.costBasis / it->qty * trade.quantity;

                    it->qty -= trade.quantity;
                    if (it->qty.isZero())
                        state.openOptionLegs.erase(it);
                }

                state.unrealizedOptionPnL -= realized;
                state.costBasis           -= costReduction;
                state.realizedCostBasis   += costReduction;
                state.realizedPnL +=
                    realized;   // no premium offset — nothing changes hands on
                                // exercise itself
                break;
            }
        }

        return state;
    }

    PnLResult<PositionState> foldEvents(
        PositionState                  state,
        std::span<const PositionEvent> events
    )
    {
        for (const auto& event : events)
        {
            if (const auto* stock = std::get_if<StockTrade>(&event.data))
            {
                state = apply(std::move(state), *stock);
            }
            else
            {
                const auto& option = std::get<OptionTrade>(event.data);
                auto        result = apply(std::move(state), option);
                if (!result)
                    return result.error();
                state = std::move(result.value());
            }
        }
        return state;
    }

    PositionPnl snapshot(
        const PositionState& state,
        std::optional<Cash>  markPrice
    )
    {
        Cash unrealized =
            state
                .unrealizedOptionPnL;   // premium-based baseline from open legs

        if (markPrice && !state.openQuantity.isZero())
        {
            const auto avgCost = state.costBasis / state.openQuantity.abs();
            unrealized += (markPrice.value() - avgCost) * state.openQuantity;
        }

        // add intrinsic value of open option legs against current underlying
        // price, mirroring your existing PnLOption::getUnrealizedPnL — same
        // caveat: needs markPrice
        if (markPrice)
        {
            for (const auto& leg : state.openOptionLegs)
            {
                Cash intrinsic{markPrice->getCurrency(), 0};
                switch (leg.type)
                {
                    case OptionType::Call:
                        intrinsic = Cash::max(
                            Cash{markPrice->getCurrency(), 0},
                            (*markPrice - leg.strikePrice) *
                                (leg.qty * state.contractSize)
                        );
                        break;
                    case OptionType::Put:
                        intrinsic = Cash::max(
                            Cash{markPrice->getCurrency(), 0},
                            (leg.strikePrice - *markPrice) *
                                (leg.qty * state.contractSize)
                        );
                        break;
                }
                unrealized += (leg.buySell == OptionBuySell::Buy) ? intrinsic
                                                                  : -intrinsic;
            }
        }

        return PositionPnl{
            .quantity          = state.openQuantity,
            .costBasis         = state.costBasis,
            .realizedPnL       = state.realizedPnL,
            .realizedCostBasis = state.realizedCostBasis,
            .unrealizedPnL     = unrealized,
            .fees              = state.fees
        };
    }

}   // namespace finance

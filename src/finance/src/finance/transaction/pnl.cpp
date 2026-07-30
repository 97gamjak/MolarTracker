#include "finance/transaction/pnl.hpp"

#include <algorithm>

#include "common/cash.hpp"
#include "common/finance.hpp"
#include "error/finance_error.hpp"

namespace finance
{

    /**
     * @brief Construct a new Open Option Leg:: Open Option Leg object
     *
     * @param type_ The type of the option (call or put)
     * @param buySell_ The buy/sell direction of the option
     * @param strikePrice_ The strike price of the option
     * @param qty_ The quantity of the option
     */
    OpenOptionLeg::OpenOptionLeg(
        OptionType    type_,
        OptionBuySell buySell_,
        Cash          strikePrice_,
        Quantity      qty_
    )
        : type(type_), buySell(buySell_), strikePrice(strikePrice_), qty(qty_)
    {
    }

    /**
     * @brief Sorts the position events in ascending order based on their
     * timestamps, allowing for chronological analysis of the events.
     */
    void PositionEvents::sort()
    {
        std::ranges::sort(
            *this,
            [](const auto& lhs, const auto& rhs)
            { return lhs.timestamp < rhs.timestamp; }
        );
    }

    namespace
    {
        /**
         * @brief Applies a stock trade to the given position state, updating
         * the state based on the trade details.
         *
         * @param state The current position state to be updated.
         * @param trade The stock trade to be applied to the position state.
         * @return PositionState The updated position state after applying the
         * stock trade.
         */
        [[nodiscard]]
        PositionState apply(PositionState state, const StockTrade& trade)
        {
            state.fees += trade.fees;

            if (trade.quantity.isZero())
                return state;

            const bool isStateQtyPositive = state.openQuantity > 0;
            const bool isTradeQtyPositive = trade.quantity > 0;
            const bool sameDirection =
                state.openQuantity.isZero() ||
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

                state.realizedPnL += (trade.unitPrice - avgCost) * closedQty;
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

        /**
         * @brief Applies an opening option trade to the given position state,
         * updating the state based on the trade details.
         *
         * @param state The current position state to be updated.
         * @param trade The option trade to be applied to the position state.
         */
        void applyOpen(PositionState& state, const OptionTrade& trade)
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

            if (trade.buySell == OptionBuySell::Buy)
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
        }

        /**
         * @brief Applies a closing option trade to the given position state,
         * updating the state based on the trade details.
         *
         * @param state The current position state to be updated.
         * @param trade The option trade to be applied to the position state.
         */
        void applyClose(PositionState& state, const OptionTrade& trade)
        {
            const auto openSide = (trade.buySell == OptionBuySell::Buy)
                                      ? OptionBuySell::Sell
                                      : OptionBuySell::Buy;
            auto       it       = std::ranges::find_if(
                state.openOptionLegs,
                [&](const auto& leg)
                {
                    return leg.type == trade.type && leg.buySell == openSide &&
                           leg.strikePrice == trade.strike;
                }
            );

            Cash realized;
            Cash costReduction;
            if (it != state.openOptionLegs.end() && !it->qty.isZero())
            {
                realized = state.unrealizedOptionPnL / it->qty * trade.quantity;
                costReduction = state.costBasis / it->qty * trade.quantity;

                it->qty -= trade.quantity;
                if (it->qty.isZero())
                    state.openOptionLegs.erase(it);
            }

            state.unrealizedOptionPnL -= realized;
            state.costBasis           -= costReduction;
            state.realizedCostBasis   += costReduction;
            state.realizedPnL         += (trade.buySell == OptionBuySell::Buy)
                                             ? (realized - trade.premium)
                                             : (realized + trade.premium);
        }

        /**
         * @brief Applies an exercised option trade to the given position state,
         * updating the state based on the trade details.
         *
         * @param state The current position state to be updated.
         * @param trade The option trade to be applied to the position state.
         */
        void applyExercised(PositionState& state, const OptionTrade& trade)
        {
            const auto openSide = (trade.buySell == OptionBuySell::Buy)
                                      ? OptionBuySell::Sell
                                      : OptionBuySell::Buy;
            auto       it       = std::ranges::find_if(
                state.openOptionLegs,
                [&](const auto& leg)
                {
                    return leg.type == trade.type && leg.buySell == openSide &&
                           leg.strikePrice == trade.strike;
                }
            );

            Cash realized;
            Cash costReduction;
            if (it != state.openOptionLegs.end() && !it->qty.isZero())
            {
                realized = state.unrealizedOptionPnL / it->qty * trade.quantity;
                costReduction = state.costBasis / it->qty * trade.quantity;

                it->qty -= trade.quantity;
                if (it->qty.isZero())
                    state.openOptionLegs.erase(it);
            }

            state.unrealizedOptionPnL -= realized;
            state.costBasis           -= costReduction;
            state.realizedCostBasis   += costReduction;
            state.realizedPnL += realized;   // no premium offset — nothing
                                             // changes hands on exercise itself
        }

        /**
         * @brief Applies an option trade to the given position state, updating
         * the state based on the trade details.
         *
         * @param state The current position state to be updated.
         * @param trade The option trade to be applied to the position state.
         * @return PnLResult<PositionState> The updated position state after
         * applying the option trade, or an error if the contract size is
         * inconsistent.
         */
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
                    applyOpen(state, trade);
                    break;
                }

                case Close:
                case RollClose:
                {
                    applyClose(state, trade);
                    break;
                }

                case Exercised:
                {
                    applyExercised(state, trade);
                    break;
                }
            }

            return state;
        }
    }   // namespace

    /**
     * @brief Folds a sequence of position events into a single position state,
     * applying each event in order to update the state.
     *
     * @param state The initial position state to start folding from.
     * @param events A span of position events to be applied to the state.
     * @return PnLResult<PositionState> The final position state after
     * applying all events, or an error if any event application fails.
     */
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

    /**
     * @brief Takes a snapshot of the current position state, calculating the
     * profit and loss (PnL) based on the state and an optional mark price.
     *
     * @param state The current position state to take a snapshot of.
     * @param markPrice An optional mark price to use for calculating
     * unrealized PnL.
     * @return PositionPnl The calculated profit and loss (PnL) for the
     * position.
     */
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

    /**
     * @brief Get the average cost of the position, which is calculated as the
     * cost basis divided by the quantity of the position, representing the
     * average price paid per share or contract in the position.
     *
     * @return Cash The average cost of the position.
     */
    Cash PositionPnl::getAverageCost() const
    {
        return quantity.isZero() ? Cash{costBasis.getCurrency(), 0}
                                 : costBasis / quantity;
    }

    /**
     * @brief Get the realized profit and loss (PnL) percentage of the position,
     * which is calculated as the realized PnL divided by the realized cost
     * basis, representing the percentage gain or loss on closed trades in the
     * position.
     *
     * @return Percentage The realized PnL percentage of the position.
     */
    Percentage PositionPnl::getRealizedPnLPercentage() const
    {
        if (realizedCostBasis.isZero())
            return Percentage(0);
        return Percentage(realizedPnL / realizedCostBasis);
    }

    /**
     * @brief Get the total profit and loss (PnL) of the position, which is
     * calculated as the sum of the realized PnL and unrealized PnL,
     * representing the overall gain or loss on both closed and open trades in
     * the position.
     *
     * @return Cash The total PnL of the position.
     */
    Cash PositionPnl::totalPnL() const { return realizedPnL + unrealizedPnL; }

    /**
     * @brief Get the market value of the position, which is calculated as the
     * cost basis plus the total PnL, representing the current value of the
     * position based on both closed and open trades.
     *
     * @return Cash The market value of the position.
     */
    Cash PositionPnl::getMarketValue() const { return costBasis + totalPnL(); }

    /**
     * @brief Get the unrealized profit and loss (PnL) percentage of the
     * position, which is calculated as the unrealized PnL divided by the cost
     * basis, representing the percentage gain or loss on open trades in the
     * position.
     *
     * @return Percentage The unrealized PnL percentage of the position.
     */
    Percentage PositionPnl::getUnrealizedPnLPercentage() const
    {
        if (costBasis.isZero())
            return Percentage(0);
        return Percentage(unrealizedPnL / costBasis);
    }

}   // namespace finance

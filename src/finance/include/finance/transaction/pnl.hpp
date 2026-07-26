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

    /**
     * @brief Represents a trade of a stock, which is a financial instrument
     * that represents ownership in a company and constitutes a claim on part of
     * the company's assets and earnings. The StockTrade struct encapsulates the
     * details of the stock trade, including the quantity of shares traded, the
     * unit price at which the shares were bought or sold, and any fees
     * associated with the trade.
     *
     */
    struct StockTrade
    {
        /// The quantity of shares traded in the stock trade
        Quantity quantity;
        /// The unit price at which the shares were bought or sold in the stock
        /// trade
        Cash unitPrice;
        /// The fees associated with the stock trade, which may include broker
        /// fees, transaction fees, or other costs incurred during the execution
        /// of the trade
        Cash fees;
    };

    /**
     * @brief Represents a trade of an option, which is a financial derivative
     * that gives the holder the right, but not the obligation, to buy or sell
     * an underlying asset at a specified price (strike price) on or before a
     * specified date (expiration date). The OptionTrade struct encapsulates the
     * details of the option trade, including its type (call or put), buy/sell
     * direction, action (open, close, roll), strike price, quantity, contract
     * size, premium paid or received, and fees associated with the trade.
     *
     */
    struct OptionTrade
    {
        /// The type of the option (call or put) for the option trade
        OptionType type;
        /// The buy/sell direction of the option (buy or sell) for the option
        /// trade
        OptionBuySell buySell;
        /// The action associated with the option trade (open, close, roll)
        TransactionOptionAction action;
        /// The strike price of the option for the option trade
        Cash strike;
        /// The quantity of the option for the option trade
        Quantity quantity;
        /// The contract size of the option for the option trade
        std::int64_t contractSize;
        /// The premium paid or received for the option trade
        Cash premium;
        /// The fees associated with the option trade
        Cash fees;
    };

    /**
     * @brief Represents an event in a financial position, which can be either a
     * stock trade or an option trade. The PositionEvent struct encapsulates the
     * details of the event, including its timestamp and the specific trade
     * data, allowing for tracking and processing of individual trades that
     * affect the state of a financial position.
     *
     */
    struct PositionEvent
    {
        /// The timestamp of the position event, indicating when the trade
        /// occurred
        Timestamp timestamp;
        /// The trade data associated with the position event, which can be
        /// either a stock trade or an option trade, represented as a variant
        /// type
        std::variant<StockTrade, OptionTrade> data;
    };

    /**
     * @brief Represents a collection of position events, which are individual
     * trades or actions that affect the state of a financial position. The
     * PositionEvents class extends a vector of PositionEvent and provides
     * methods to sort the events by timestamp, allowing for chronological
     * processing of the events to determine the overall impact on the position.
     *
     */
    class PositionEvents : public Vector<PositionEvent>
    {
       public:
        using Vector<PositionEvent>::Vector;

        void sort();
    };

    /**
     * @brief Represents an open option leg in a position, which is a component
     * of the position that corresponds to an individual option contract that
     * has been opened (i.e., bought or sold) and is still active in the
     * position. The OpenOptionLeg struct encapsulates the properties of an open
     * option leg, such as its type (call or put), buy/sell direction, strike
     * price, and quantity, and provides methods to access these properties and
     * perform operations related to the open option leg.
     *
     */
    struct OpenOptionLeg
    {
        /// The type of the option (call or put) for the open option leg
        OptionType type;
        /// The buy/sell direction of the option (buy or sell) for the open
        OptionBuySell buySell;
        /// The strike price of the option for the open option leg
        Cash strikePrice;
        /// The quantity of the option for the open option leg
        Quantity qty;

        OpenOptionLeg(
            OptionType    type_,
            OptionBuySell buySell_,
            Cash          strikePrice_,
            Quantity      qty_
        );
    };

    /**
     * @brief Represents the state of a financial position, which includes the
     * quantity of the position, cost basis, realized PnL, realized cost basis,
     * unrealized PnL, fees, contract size, and any open option legs associated
     * with the position. The PositionState struct encapsulates these properties
     * and provides methods to access and manipulate the state of the position,
     * allowing for tracking and analysis of the financial performance of the
     * position over time.
     *
     */
    struct PositionState
    {
        /// The quantity of the position, representing the number of shares or
        /// contracts held in the position
        Quantity openQuantity{0};
        /// The cost basis of the position, representing the total amount paid
        /// for the shares or contracts held in the position
        Cash costBasis;
        /// The realized profit and loss (PnL) of the position, representing the
        /// profit or loss that has been realized from closed trades in the
        /// position
        Cash realizedPnL;
        /// The realized cost basis of the position, representing the total
        /// amount paid for the shares or contracts that have been closed in the
        /// position
        Cash realizedCostBasis;
        /// The unrealized profit and loss (PnL) of the position, representing
        /// the profit or loss that has not yet been realized from open trades
        /// in the position
        Cash unrealizedOptionPnL;
        /// The fees associated with the position, representing the total costs
        /// incurred from trading activities in the position, including broker
        /// fees, transaction fees, and other costs
        Cash fees;
        /// The contract size of the position, representing the number of units
        /// of the underlying asset that each option contract represents, which
        /// is used to calculate the total value of the position and its
        /// associated PnL
        std::int64_t contractSize{0};
        /// The collection of open option legs associated with the position,
        /// representing the individual option contracts that have been opened
        /// and are still active in the position, allowing for tracking and
        /// analysis of the specific option trades that contribute to the
        /// overall state of the position
        std::vector<OpenOptionLeg> openOptionLegs;
    };

    /**
     * @brief Represents the profit and loss (PnL) of a financial position,
     * which includes the quantity of the position, cost basis, realized PnL,
     * realized cost basis, unrealized PnL, and fees associated with the
     * position. The PositionPnl struct encapsulates these properties and
     * provides methods to calculate average cost, realized PnL percentage,
     * total PnL, market value, and unrealized PnL percentage for the position.
     *
     */
    struct PositionPnl
    {
        /// The quantity of the position, representing the number of shares or
        /// contracts held in the position
        Quantity quantity;
        /// The cost basis of the position, representing the total amount paid
        /// for the shares or contracts held in the position
        Cash costBasis;
        /// The realized profit and loss (PnL) of the position, representing the
        /// profit or loss that has been realized from closed trades in the
        /// position
        Cash realizedPnL;
        /// The realized cost basis of the position, representing the total
        /// amount paid for the shares or contracts that have been closed in the
        /// position
        Cash realizedCostBasis;
        /// The unrealized profit and loss (PnL) of the position, representing
        /// the profit or loss that has not yet been realized from open trades
        /// in the position
        Cash unrealizedPnL;
        /// The fees associated with the position, representing the total costs
        /// incurred from trading activities in the position, including broker
        /// fees, transaction fees, and other costs
        Cash fees;

        [[nodiscard]] Cash       getAverageCost() const;
        [[nodiscard]] Percentage getRealizedPnLPercentage() const;
        [[nodiscard]] Cash       totalPnL() const;
        [[nodiscard]] Cash       getMarketValue() const;
        [[nodiscard]] Percentage getUnrealizedPnLPercentage() const;
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
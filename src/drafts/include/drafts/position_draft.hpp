#ifndef __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__

#include "common/cash.hpp"
#include "common/percentage.hpp"
#include "common/quantity.hpp"
#include "common/timestamp.hpp"
#include "config/id_types.hpp"
#include "drafts/stock_draft.hpp"

namespace drafts
{
    /**
     * @brief Represents a draft for a position in the trading system
     *
     */
    class PositionDraft
    {
       private:
        /// The position ID of the position draft
        PositionId _positionId;

        /// The stock information of the position draft
        StockInfoDraft _stockInfo;

        /// The creation timestamp of the position draft
        Timestamp _createdAt;

        /// The closing timestamp of the position draft, if it exists
        std::optional<Timestamp> _closedAt;

       public:
        explicit PositionDraft(
            PositionId               positionId,
            StockInfoDraft           stockInfo,
            Timestamp                createdAt,
            std::optional<Timestamp> closedAt = std::nullopt
        );

        [[nodiscard]] Timestamp getCreatedAt() const;

        [[nodiscard]] std::optional<Timestamp> getClosedAt() const;

        [[nodiscard]] StockInfoDraft getStockInfo() const;

        [[nodiscard]] PositionId getPositionId() const;
    };

    /**
     * @brief Represents a detailed draft for a stock position in the trading
     * system
     *
     */
    class PositionStockDetailDraft : public PositionDraft
    {
       private:
        /// The quantity of the position
        Quantity _quantity;

        /// The current price of the stock in the position
        Cash _currentPrice;

        /// The current market value of the position
        Cash _marketValue;

        /// The average price of the stock in the position
        Cash _averagePrice;

        /// The total price of the stock in the position
        Cash _totalPrice;

        /// The realized profit and loss (PnL) of the position
        Cash _realizedPnL;

        /// The realized profit and loss (PnL) percentage of the position
        Percentage _realizedPnLPercentage = Percentage(0.0);

        /// The unrealized profit and loss (PnL) of the position
        Cash _unrealizedPnL;

        /// The unrealized profit and loss (PnL) percentage of the position
        Percentage _unrealizedPnLPercentage = Percentage(0.0);

       public:
        explicit PositionStockDetailDraft(
            PositionId               positionId,
            StockInfoDraft           stockInfo,
            Timestamp                createdAt,
            Quantity                 quantity,
            Cash                     averagePrice,
            Cash                     totalPrice,
            Cash                     realizedPnL,
            Percentage               realizedPnLPercentage,
            std::optional<Timestamp> closedAt = std::nullopt
        );

        void updateUnrealizedPnL(
            const Cash& currentPrice,
            const Cash& marketValue,
            const Cash& unrealizedPnL,
            Percentage  unrealizedPnLPercentage
        );

        [[nodiscard]] Quantity   getQuantity() const;
        [[nodiscard]] Cash       getCurrentPrice() const;
        [[nodiscard]] Cash       getMarketValue() const;
        [[nodiscard]] Cash       getAveragePrice() const;
        [[nodiscard]] Cash       getTotalPrice() const;
        [[nodiscard]] Cash       getRealizedPnL() const;
        [[nodiscard]] Percentage getRealizedPnLPercentage() const;
        [[nodiscard]] Cash       getUnrealizedPnL() const;
        [[nodiscard]] Percentage getUnrealizedPnLPercentage() const;
    };
}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__
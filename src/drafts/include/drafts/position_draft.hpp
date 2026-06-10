#ifndef __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__

#include "config/id_types.hpp"
#include "config/quantity.hpp"
#include "drafts/stock_draft.hpp"
#include "finance/cash.hpp"
#include "utils/percentage.hpp"
#include "utils/timestamp.hpp"

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
        finance::Cash _currentPrice;

        /// The current market value of the position
        finance::Cash _marketValue;

        /// The average price of the stock in the position
        finance::Cash _averagePrice;

        /// The total price of the stock in the position
        finance::Cash _totalPrice;

        /// The realized profit and loss (PnL) of the position
        finance::Cash _realizedPnL;

        /// The realized profit and loss (PnL) percentage of the position
        Percentage _realizedPnLPercentage = Percentage(0.0);

        /// The unrealized profit and loss (PnL) of the position
        finance::Cash _unrealizedPnL;

        /// The unrealized profit and loss (PnL) percentage of the position
        Percentage _unrealizedPnLPercentage = Percentage(0.0);

       public:
        explicit PositionStockDetailDraft(
            PositionId               positionId,
            StockInfoDraft           stockInfo,
            Timestamp                createdAt,
            Quantity                 quantity,
            finance::Cash            averagePrice,
            finance::Cash            totalPrice,
            finance::Cash            realizedPnL,
            Percentage               realizedPnLPercentage,
            std::optional<Timestamp> closedAt = std::nullopt
        );

        void updateUnrealizedPnL(
            const finance::Cash& currentPrice,
            const finance::Cash& marketValue,
            const finance::Cash& unrealizedPnL,
            Percentage           unrealizedPnLPercentage
        );

        [[nodiscard]] Quantity      getQuantity() const;
        [[nodiscard]] finance::Cash getCurrentPrice() const;
        [[nodiscard]] finance::Cash getMarketValue() const;
        [[nodiscard]] finance::Cash getAveragePrice() const;
        [[nodiscard]] finance::Cash getTotalPrice() const;
        [[nodiscard]] finance::Cash getRealizedPnL() const;
        [[nodiscard]] Percentage    getRealizedPnLPercentage() const;
        [[nodiscard]] finance::Cash getUnrealizedPnL() const;
        [[nodiscard]] Percentage    getUnrealizedPnLPercentage() const;
    };
}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__
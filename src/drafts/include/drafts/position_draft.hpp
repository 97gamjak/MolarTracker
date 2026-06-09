#ifndef __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__

#include "config/id_types.hpp"
#include "config/quantity.hpp"
#include "drafts/stock_draft.hpp"
#include "finance/cash.hpp"
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

        finance::Cash _averagePrice;

        finance::Cash _totalPrice;

        finance::Cash _realizedPnL;

        double _realizedPnLPercentage;

        finance::Cash _unrealizedPnL;

        double _unrealizedPnLPercentage;

       public:
        explicit PositionStockDetailDraft(
            PositionId               positionId,
            StockInfoDraft           stockInfo,
            Timestamp                createdAt,
            Quantity                 quantity,
            finance::Cash            averagePrice,
            finance::Cash            totalPrice,
            finance::Cash            realizedPnL,
            double                   realizedPnLPercentage,
            std::optional<Timestamp> closedAt = std::nullopt
        );

        void updateUnrealizedPnL(
            const finance::Cash& unrealizedPnL,
            double               unrealizedPnLPercentage
        );

        [[nodiscard]] Quantity      getQuantity() const;
        [[nodiscard]] finance::Cash getAveragePrice() const;
        [[nodiscard]] finance::Cash getTotalPrice() const;
        [[nodiscard]] finance::Cash getRealizedPnL() const;
        [[nodiscard]] double        getRealizedPnLPercentage() const;
        [[nodiscard]] finance::Cash getUnrealizedPnL() const;
        [[nodiscard]] double        getUnrealizedPnLPercentage() const;
    };
}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__
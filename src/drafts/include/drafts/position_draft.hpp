#ifndef __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__

#include "config/id_types.hpp"
#include "config/quantity.hpp"
#include "drafts/stock_draft.hpp"
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
     * @brief Represents a detailed draft for a position in the trading system
     *
     */
    class PositionDetailDraft : public PositionDraft
    {
       private:
        /// The quantity of the position
        Quantity _quantity;

       public:
        explicit PositionDetailDraft(
            PositionId               positionId,
            StockInfoDraft           stockInfo,
            Timestamp                createdAt,
            Quantity                 quantity,
            std::optional<Timestamp> closedAt = std::nullopt
        );

        [[nodiscard]] Quantity getQuantity() const;
    };
}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__
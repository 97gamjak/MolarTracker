#ifndef __DRAFTS__INCLUDE__DRAFTS__POSITION__POSITION_OPTION_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__POSITION__POSITION_OPTION_DRAFT_HPP__

#include "common/cash.hpp"
#include "common/percentage.hpp"
#include "common/quantity.hpp"
#include "position_draft.hpp"

namespace drafts
{
    /**
     * @brief Represents a detailed draft for an option position in the trading
     * system
     *
     */
    class PositionOptionDetailDraft : public PositionDraft
    {
       private:
        /// The quantity of the position
        Quantity _quantity;

        /// The current price of the underlying stock in the position
        Cash _currentPrice;

        /// The current market value of the position
        Cash _marketValue;

        /// The realized profit and loss (PnL) of the position
        Cash _realizedPnL;

        /// The realized profit and loss (PnL) percentage of the position
        Percentage _realizedPnLPercentage = Percentage(0.0);

        /// The unrealized profit and loss (PnL) of the position
        Cash _unrealizedPnL;

        /// The unrealized profit and loss (PnL) percentage of the position
        Percentage _unrealizedPnLPercentage = Percentage(0.0);

       public:
        explicit PositionOptionDetailDraft(
            PositionId               positionId,
            StockInfoDraft           stockInfo,
            Timestamp                createdAt,
            Quantity                 quantity,
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
        [[nodiscard]] Cash       getRealizedPnL() const;
        [[nodiscard]] Percentage getRealizedPnLPercentage() const;
        [[nodiscard]] Cash       getUnrealizedPnL() const;
        [[nodiscard]] Percentage getUnrealizedPnLPercentage() const;
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__POSITION__POSITION_OPTION_DRAFT_HPP__
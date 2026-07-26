#include "drafts/position/position_option_draft.hpp"

namespace drafts
{
    /**
     * @brief Construct a new Position Option Detail Draft:: Position Option
     * Detail Draft object
     *
     * @param positionId
     * @param stockInfo
     * @param createdAt
     * @param quantity
     * @param realizedPnL
     * @param realizedPnLPercentage
     * @param closedAt
     */
    PositionOptionDetailDraft::PositionOptionDetailDraft(
        PositionId               positionId,
        StockInfoDraft           stockInfo,
        Timestamp                createdAt,
        Quantity                 quantity,
        Cash                     realizedPnL,
        Percentage               realizedPnLPercentage,
        std::optional<Timestamp> closedAt
    )
        : PositionDraft(positionId, std::move(stockInfo), createdAt, closedAt),
          _quantity(quantity),
          _realizedPnL(realizedPnL),
          _realizedPnLPercentage(realizedPnLPercentage)
    {
    }

    /**
     * @brief Update the unrealized PnL information for the position detail
     * draft
     *
     * @param currentPrice The current price of the stock
     * @param marketValue The current market value of the position
     * @param unrealizedPnL The unrealized PnL of the position
     * @param unrealizedPnLPercentage The unrealized PnL percentage of the
     * position
     */
    void PositionOptionDetailDraft::updateUnrealizedPnL(
        const Cash& currentPrice,
        const Cash& marketValue,
        const Cash& unrealizedPnL,
        Percentage  unrealizedPnLPercentage
    )
    {
        _currentPrice            = currentPrice;
        _marketValue             = marketValue;
        _unrealizedPnL           = unrealizedPnL;
        _unrealizedPnLPercentage = unrealizedPnLPercentage;
    }

    /**
     * @brief Get the quantity of the position detail draft
     *
     * @return Quantity The quantity
     */
    Quantity PositionOptionDetailDraft::getQuantity() const
    {
        return _quantity;
    }

    /**
     * @brief Get the current price of the position detail draft
     *
     * @return Cash The current price
     */
    Cash PositionOptionDetailDraft::getCurrentPrice() const
    {
        return _currentPrice;
    }

    /**
     * @brief Get the market value of the position detail draft
     *
     * @return Cash The market value
     */
    Cash PositionOptionDetailDraft::getMarketValue() const
    {
        return _marketValue;
    }

    /**
     * @brief Get the realized PnL of the position detail draft
     *
     * @return Cash The realized PnL
     */
    Cash PositionOptionDetailDraft::getRealizedPnL() const
    {
        return _realizedPnL;
    }

    /**
     * @brief Get the realized PnL percentage of the position detail draft
     *
     * @return Percentage The realized PnL percentage
     */
    Percentage PositionOptionDetailDraft::getRealizedPnLPercentage() const
    {
        return _realizedPnLPercentage;
    }

    /**
     * @brief Get the unrealized PnL of the position detail draft
     *
     * @return Cash The unrealized PnL
     */
    Cash PositionOptionDetailDraft::getUnrealizedPnL() const
    {
        return _unrealizedPnL;
    }

    /**
     * @brief Get the unrealized PnL percentage of the position detail draft
     *
     * @return Percentage The unrealized PnL percentage
     */
    Percentage PositionOptionDetailDraft::getUnrealizedPnLPercentage() const
    {
        return _unrealizedPnLPercentage;
    }
}   // namespace drafts
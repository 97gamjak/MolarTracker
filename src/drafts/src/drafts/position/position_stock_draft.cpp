#include "drafts/position/position_stock_draft.hpp"

namespace drafts
{
    /**
     * @brief Construct a new Position Detail Draft:: Position Detail Draft
     * object
     *
     * @param positionId
     * @param stockInfo
     * @param createdAt
     * @param quantity
     * @param averagePrice
     * @param totalPrice
     * @param realizedPnL
     * @param realizedPnLPercentage
     * @param closedAt
     */
    PositionStockDetailDraft::PositionStockDetailDraft(
        PositionId               positionId,
        StockInfoDraft           stockInfo,
        Timestamp                createdAt,
        Quantity                 quantity,
        Cash                     averagePrice,
        Cash                     totalPrice,
        Cash                     realizedPnL,
        Percentage               realizedPnLPercentage,
        std::optional<Timestamp> closedAt
    )
        : PositionDraft(positionId, std::move(stockInfo), createdAt, closedAt),
          _quantity(quantity),
          _averagePrice(averagePrice),
          _totalPrice(totalPrice),
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
    void PositionStockDetailDraft::updateUnrealizedPnL(
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
    Quantity PositionStockDetailDraft::getQuantity() const { return _quantity; }

    /**
     * @brief Get the current price of the position detail draft
     *
     * @return Cash The current price
     */
    Cash PositionStockDetailDraft::getCurrentPrice() const
    {
        return _currentPrice;
    }

    /**
     * @brief Get the market value of the position detail draft
     *
     * @return Cash The market value
     */
    Cash PositionStockDetailDraft::getMarketValue() const
    {
        return _marketValue;
    }

    /**
     * @brief Get the average price of the position detail draft
     *
     * @return Cash The average price
     */
    Cash PositionStockDetailDraft::getAveragePrice() const
    {
        return _averagePrice;
    }

    /**
     * @brief Get the total price of the position detail draft
     *
     * @return Cash The total price
     */
    Cash PositionStockDetailDraft::getTotalPrice() const { return _totalPrice; }

    /**
     * @brief Get the realized PnL of the position detail draft
     *
     * @return Cash The realized PnL
     */
    Cash PositionStockDetailDraft::getRealizedPnL() const
    {
        return _realizedPnL;
    }

    /**
     * @brief Get the realized PnL percentage of the position detail draft
     *
     * @return Percentage The realized PnL percentage
     */
    Percentage PositionStockDetailDraft::getRealizedPnLPercentage() const
    {
        return _realizedPnLPercentage;
    }

    /**
     * @brief Get the unrealized PnL of the position detail draft
     *
     * @return Cash The unrealized PnL
     */
    Cash PositionStockDetailDraft::getUnrealizedPnL() const
    {
        return _unrealizedPnL;
    }

    /**
     * @brief Get the unrealized PnL percentage of the position detail draft
     *
     * @return Percentage The unrealized PnL percentage
     */
    Percentage PositionStockDetailDraft::getUnrealizedPnLPercentage() const
    {
        return _unrealizedPnLPercentage;
    }
}   // namespace drafts
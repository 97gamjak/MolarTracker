#include "drafts/position_draft.hpp"

#include "config/quantity.hpp"
#include "drafts/stock_draft.hpp"

namespace drafts
{
    /**
     * @brief Construct a new Position Draft object
     *
     * @param positionId
     * @param stockInfo
     * @param createdAt
     * @param closedAt
     */
    PositionDraft::PositionDraft(
        PositionId               positionId,
        StockInfoDraft           stockInfo,
        Timestamp                createdAt,
        std::optional<Timestamp> closedAt
    )
        : _positionId(positionId),
          _stockInfo(std::move(stockInfo)),
          _createdAt(createdAt),
          _closedAt(closedAt)
    {
    }

    /**
     * @brief Get the creation timestamp of the position draft
     *
     * @return Timestamp The creation timestamp
     */
    Timestamp PositionDraft::getCreatedAt() const { return _createdAt; }

    /**
     * @brief Get the closing timestamp of the position draft
     *
     * @return std::optional<Timestamp> The closing timestamp, if it exists
     */
    std::optional<Timestamp> PositionDraft::getClosedAt() const
    {
        return _closedAt;
    }

    /**
     * @brief Get the stock information of the position draft
     *
     * @return StockInfoDraft The stock information
     */
    StockInfoDraft PositionDraft::getStockInfo() const { return _stockInfo; }

    /**
     * @brief Get the position ID of the position draft
     *
     * @return PositionId The position ID
     */
    PositionId PositionDraft::getPositionId() const { return _positionId; }

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
        finance::Cash            averagePrice,
        finance::Cash            totalPrice,
        finance::Cash            realizedPnL,
        double                   realizedPnLPercentage,
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
        const finance::Cash& currentPrice,
        const finance::Cash& marketValue,
        const finance::Cash& unrealizedPnL,
        double               unrealizedPnLPercentage
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
     * @return finance::Cash The current price
     */
    finance::Cash PositionStockDetailDraft::getCurrentPrice() const
    {
        return _currentPrice;
    }

    /**
     * @brief Get the market value of the position detail draft
     *
     * @return finance::Cash The market value
     */
    finance::Cash PositionStockDetailDraft::getMarketValue() const
    {
        return _marketValue;
    }

    /**
     * @brief Get the average price of the position detail draft
     *
     * @return finance::Cash The average price
     */
    finance::Cash PositionStockDetailDraft::getAveragePrice() const
    {
        return _averagePrice;
    }

    /**
     * @brief Get the total price of the position detail draft
     *
     * @return finance::Cash The total price
     */
    finance::Cash PositionStockDetailDraft::getTotalPrice() const
    {
        return _totalPrice;
    }

    /**
     * @brief Get the realized PnL of the position detail draft
     *
     * @return finance::Cash The realized PnL
     */
    finance::Cash PositionStockDetailDraft::getRealizedPnL() const
    {
        return _realizedPnL;
    }

    /**
     * @brief Get the realized PnL percentage of the position detail draft
     *
     * @return double The realized PnL percentage
     */
    double PositionStockDetailDraft::getRealizedPnLPercentage() const
    {
        return _realizedPnLPercentage;
    }

    /**
     * @brief Get the unrealized PnL of the position detail draft
     *
     * @return finance::Cash The unrealized PnL
     */
    finance::Cash PositionStockDetailDraft::getUnrealizedPnL() const
    {
        return _unrealizedPnL;
    }

    /**
     * @brief Get the unrealized PnL percentage of the position detail draft
     *
     * @return double The unrealized PnL percentage
     */
    double PositionStockDetailDraft::getUnrealizedPnLPercentage() const
    {
        return _unrealizedPnLPercentage;
    }

}   // namespace drafts

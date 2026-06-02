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
     * @param closedAt
     */
    PositionStockDetailDraft::PositionStockDetailDraft(
        PositionId               positionId,
        StockInfoDraft           stockInfo,
        Timestamp                createdAt,
        Quantity                 quantity,
        std::optional<Timestamp> closedAt
    )
        : PositionDraft(positionId, std::move(stockInfo), createdAt, closedAt),
          _quantity(quantity)
    {
    }

    /**
     * @brief Get the quantity of the position detail draft
     *
     * @return Quantity The quantity
     */
    Quantity PositionStockDetailDraft::getQuantity() const { return _quantity; }

}   // namespace drafts

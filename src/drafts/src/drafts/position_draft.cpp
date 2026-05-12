#include "drafts/position_draft.hpp"

#include "drafts/stock_draft.hpp"

namespace drafts
{
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

    Timestamp PositionDraft::getCreatedAt() const { return _createdAt; }

    std::optional<Timestamp> PositionDraft::getClosedAt() const
    {
        return _closedAt;
    }

    StockInfoDraft PositionDraft::getStockInfo() const { return _stockInfo; }

    PositionId PositionDraft::getPositionId() const { return _positionId; }

}   // namespace drafts

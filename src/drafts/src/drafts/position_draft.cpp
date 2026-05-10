#include "drafts/position_draft.hpp"

#include <memory>

#include "drafts/stock_draft.hpp"

namespace drafts
{
    PositionDraft::PositionDraft(
        StockInfoDraft           stockInfo,
        Timestamp                createdAt,
        std::optional<Timestamp> closedAt
    )
        : _stockDraft(std::make_unique<StockInfoDraft>(std::move(stockInfo))),
          _createdAt(createdAt),
          _closedAt(closedAt)
    {
    }

    Timestamp PositionDraft::getCreatedAt() const { return _createdAt; }

    std::optional<Timestamp> PositionDraft::getClosedAt() const
    {
        return _closedAt;
    }

}   // namespace drafts

#ifndef __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__

#include <memory>

#include "utils/timestamp.hpp"

namespace drafts
{
    class StockInfoDraft;   // Forward declaration

    class PositionDraft
    {
       private:
        std::unique_ptr<StockInfoDraft> _stockDraft;

        /// The creation timestamp of the position draft
        Timestamp _createdAt;

        /// The closing timestamp of the position draft, if it exists
        std::optional<Timestamp> _closedAt;

       public:
        explicit PositionDraft(
            StockInfoDraft           stockInfo,
            Timestamp                createdAt,
            std::optional<Timestamp> closedAt = std::nullopt
        );

        [[nodiscard]] Timestamp getCreatedAt() const;

        [[nodiscard]] std::optional<Timestamp> getClosedAt() const;
    };
}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__POSITION_DRAFT_HPP__
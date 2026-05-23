#ifndef __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__
#define __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__

#include <expected>
#include <string>
#include <vector>

#include "config/id_types.hpp"

namespace app
{
    class StockStore;         // Forward declaration
    class TransactionStore;   // Forward declaration
    class PositionStore;      // Forward declaration
}   // namespace app

namespace drafts
{
    class CreateStockTransactionDraft;   // Forward declaration
    class PositionDraft;                 // Forward declaration
    class PositionDetailDraft;           // Forward declaration
}   // namespace drafts

namespace controller
{
    std::expected<void, std::string> convertTickerToInstrumentId(
        drafts::CreateStockTransactionDraft& draft,
        const app::StockStore&               stockStore
    );

    std::vector<drafts::PositionDetailDraft> getOpenPositionDrafts(
        AccountId                    account,
        const app::PositionStore&    positionStore,
        const app::StockStore&       stockStore,
        const app::TransactionStore& transactionStore
    );

    std::vector<drafts::PositionDraft> getOpenPositionDrafts(
        const app::PositionStore&    positionStore,
        const app::StockStore&       stockStore,
        const app::TransactionStore& transactionStore
    );

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__
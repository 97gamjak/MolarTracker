#ifndef __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__
#define __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__

#include <expected>
#include <memory>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "drafts/position_draft.hpp"
#include "finance/transaction/pnl.hpp"

namespace store
{
    class IStockStore;         // Forward declaration
    class ITransactionStore;   // Forward declaration
    class IPositionStore;      // Forward declaration
}   // namespace store

namespace drafts
{
    class CreateStockTransactionDraft;   // Forward declaration
    class PositionDraft;                 // Forward declaration
    class PositionStockDetailDraft;      // Forward declaration
}   // namespace drafts

/**
 * @brief Detail struct for open stock positions, containing the position draft,
 * ticker, and PnL information.
 *
 */
struct OpenStockPositionDetail
{
    drafts::PositionStockDetailDraft positionDraft;
    std::string                      ticker;
    std::shared_ptr<finance::PnL>    pnl;
};

namespace controller
{
    std::expected<void, std::string> convertTickerToInstrumentId(
        drafts::CreateStockTransactionDraft&       draft,
        const std::shared_ptr<store::IStockStore>& stockStore
    );

    std::vector<drafts::PositionStockDetailDraft> getOpenStockPositions(
        AccountId                                        account,
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<store::IStockStore>&       stockStore,
        const std::shared_ptr<store::ITransactionStore>& transactionStore
    );

    std::vector<OpenStockPositionDetail> getOpenStockPositionDetails(
        AccountId                                        account,
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<store::IStockStore>&       stockStore,
        const std::shared_ptr<store::ITransactionStore>& transactionStore
    );

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__
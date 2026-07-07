#ifndef __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__
#define __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__

#include <memory>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "drafts/position_draft.hpp"
#include "finance/transaction/pnl.hpp"

namespace store
{
    class ITransactionStore;   // Forward declaration
    class IPositionStore;      // Forward declaration
}   // namespace store

namespace drafts
{
    class CreateStockTransactionDraft;    // Forward declaration
    class CreateOptionTransactionDraft;   // Forward declaration
    class PositionDraft;                  // Forward declaration
    class PositionStockDetailDraft;       // Forward declaration
}   // namespace drafts

namespace cache
{
    class StockCache;     // Forward declaration
    class AccountCache;   // Forward declaration
}   // namespace cache

/**
 * @brief Detail struct for open stock positions, containing the position draft,
 * ticker, and PnL information.
 *
 */
struct OpenStockPositionDetail
{
    /// The position draft containing the details of the open stock position.
    drafts::PositionStockDetailDraft positionDraft;
    /// The ticker symbol of the stock associated with the open position.
    std::string ticker;
    /// The profit and loss (PnL) information for the open stock position.
    std::shared_ptr<finance::PnL> pnl;
};

namespace controller
{
    std::vector<drafts::PositionStockDetailDraft> getOpenStockPositions(
        AccountId                                        account,
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<cache::StockCache>&        stockCache,
        const std::shared_ptr<cache::AccountCache>&      accountCache,
        const std::shared_ptr<store::ITransactionStore>& transactionStore
    );

    std::vector<OpenStockPositionDetail> getOpenStockPositionDetails(
        AccountId                                        account,
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<cache::StockCache>&        stockCache,
        const std::shared_ptr<cache::AccountCache>&      accountCache,
        const std::shared_ptr<store::ITransactionStore>& transactionStore
    );

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__
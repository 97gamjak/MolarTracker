#ifndef __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__
#define __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__

#include <expected>
#include <memory>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "drafts/position/position_option_draft.hpp"
#include "drafts/position/position_stock_draft.hpp"
#include "finance/transaction/pnl.hpp"

namespace store
{
    class IStockStore;         // Forward declaration
    class ITransactionStore;   // Forward declaration
    class IPositionStore;      // Forward declaration
    class IOptionStore;        // Forward declaration
}   // namespace store

namespace drafts
{
    class CreateStockTransactionDraft;    // Forward declaration
    class CreateOptionTransactionDraft;   // Forward declaration
    class PositionDraft;                  // Forward declaration
}   // namespace drafts

namespace gateway
{
    class PositionGateway;   // Forward declaration
}   // namespace gateway

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

/**
 * @brief Detail struct for open option positions, containing the position
 * draft, ticker, and PnL information.
 *
 */
struct OpenOptionPositionDetail
{
    /// The position draft containing the details of the open option position.
    drafts::PositionOptionDetailDraft positionDraft;
    /// The ticker symbol of the option associated with the open position.
    std::string ticker;
    /// The profit and loss (PnL) information for the open option position.
    std::shared_ptr<finance::PnL> pnl;
};

namespace controller
{
    std::expected<void, std::string> convertTickerToInstrumentId(
        drafts::CreateStockTransactionDraft&       draft,
        const std::shared_ptr<store::IStockStore>& stockStore
    );

    std::expected<void, std::string> convertTickerToInstrumentId(
        drafts::CreateOptionTransactionDraft&      draft,
        const std::shared_ptr<store::IStockStore>& stockStore
    );

    FinanceResult<std::vector<drafts::PositionStockDetailDraft>> getOpenStockPositions(
        AccountId                                        account,
        const std::shared_ptr<gateway::PositionGateway>& positionGateway,
        const std::shared_ptr<store::IStockStore>&       stockStore
    );

    FinanceResult<std::vector<OpenStockPositionDetail>> getOpenStockPositionDetails(
        AccountId                                        account,
        const std::shared_ptr<gateway::PositionGateway>& positionGateway,
        const std::shared_ptr<store::IStockStore>&       stockStore
    );

    FinanceResult<std::vector<OpenOptionPositionDetail>> getOpenOptionPositionDetails(
        AccountId                                        account,
        const std::shared_ptr<gateway::PositionGateway>& positionGateway,
        const std::shared_ptr<store::IOptionStore>&      optionStore
    );

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__
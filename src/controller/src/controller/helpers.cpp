#include "helpers.hpp"

#include <expected>

#include "config/id_types.hpp"
#include "drafts/position/position_stock_draft.hpp"
#include "drafts/transaction/transaction_create_draft.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "gateway/position_gateway.hpp"
#include "logging/log_macros.hpp"
#include "mapper/stock_mapper.hpp"
#include "store/i_option_store.hpp"
#include "store/i_position_store.hpp"
#include "store/i_stock_store.hpp"
#include "store/i_transaction_store.hpp"

REGISTER_LOG_CATEGORY("Controller.Helpers");

namespace controller
{
    /**
     * @brief Convert stock tickers to instrument IDs in the draft
     *
     * @param draft
     * @param stockStore
     * @return std::expected<void, std::string>
     */
    std::expected<void, std::string> convertTickerToInstrumentId(
        drafts::CreateStockTransactionDraft&       draft,
        const std::shared_ptr<store::IStockStore>& stockStore
    )
    {
        const auto  ticker       = draft.getTicker();
        const auto& instrumentId = stockStore->getInstrumentId(ticker);

        if (instrumentId)
            draft.setInstrumentId(*instrumentId);
        else
            return std::unexpected("Invalid stock ticker: " + ticker);

        return {};
    }

    std::expected<void, std::string> convertTickerToInstrumentId(
        drafts::CreateOptionTransactionDraft&      draft,
        const std::shared_ptr<store::IStockStore>& stockStore
    )
    {
        const auto  ticker       = draft.getUnderlyingTicker();
        const auto& instrumentId = stockStore->getInstrumentId(ticker);

        if (instrumentId)
            draft.setUnderlyingInstrumentId(*instrumentId);
        else
            return std::unexpected("Invalid stock ticker: " + ticker);

        return {};
    }

}   // namespace controller
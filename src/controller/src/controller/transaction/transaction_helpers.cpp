#include "transaction_helpers.hpp"

#include "app/store/stock_store.hpp"
#include "drafts/transaction_draft.hpp"

namespace controller
{
    std::expected<void, std::string> convertTickerToInstrumentId(
        drafts::CreateStockTransactionDraft& draft,
        const app::StockStore&               stockStore
    )
    {
        for (auto& leg : draft.getLegs())
        {
            const auto& ticker       = leg.getTicker();
            const auto& instrumentId = stockStore.getInstrumentId(ticker);

            if (instrumentId)
                leg.setInstrumentId(*instrumentId);
            else
                return std::unexpected("Invalid stock ticker: " + ticker);
        }

        return {};
    }
}   // namespace controller
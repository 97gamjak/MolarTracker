#include "transaction_helpers.hpp"

#include "drafts/transaction_draft.hpp"
#include "store/stock_store.hpp"

namespace controller
{
    std::expected<void, std::string> convertTickerToInstrumentId(
        drafts::CreateStockTransactionDraft& draft,
        const store::StockStore&             stockStore
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
#include "transaction_helpers.hpp"

#include <memory>

#include "drafts/transaction_draft.hpp"
#include "store/i_stock_store.hpp"

namespace controller
{
    /**
     * @brief Convert stock tickers in the transaction draft to instrument IDs
     * using the stock store
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
        for (auto& leg : draft.getLegs())
        {
            const auto& ticker       = leg.getTicker();
            const auto& instrumentId = stockStore->getInstrumentId(ticker);

            if (instrumentId)
                leg.setInstrumentId(*instrumentId);
            else
                return std::unexpected("Invalid stock ticker: " + ticker);
        }

        return {};
    }
}   // namespace controller
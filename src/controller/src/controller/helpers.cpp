#include "helpers.hpp"

#include "app/store/position_store.hpp"
#include "app/store/stock_store.hpp"
#include "app/store/transaction_store.hpp"
#include "drafts/position_draft.hpp"
#include "drafts/stock_mapper.hpp"
#include "drafts/transaction_draft.hpp"
#include "finance/position.hpp"

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

    namespace
    {
        /**
         * @brief Get position drafts from the given positions
         *
         * @param positions
         * @param stockStore
         * @param transactionStore
         * @return std::vector<drafts::PositionDraft>
         */
        std::vector<drafts::PositionDraft> _getPositionDrafts(
            const std::vector<finance::Position>& positions,
            const app::StockStore&                stockStore,
            const app::TransactionStore&          transactionStore
        )
        {
            std::vector<drafts::PositionDraft> drafts;
            for (const auto& position : positions)
            {
                const auto instrumentIds =
                    transactionStore.getInstrumentIdsByPositionId(
                        position.getId()
                    );

                const auto& stocks = stockStore.getStocks(instrumentIds);

                if (stocks.empty())
                {
                    LOG_WARNING(
                        "No stock found for instrument id: " +
                        position.getId().toString()
                    );
                    continue;
                }

                drafts.emplace_back(
                    position.getId(),
                    drafts::StockMapper::toStockInfoDraft(stocks.front()),
                    position.getCreatedAt()
                );
            }

            return drafts;
        }
    }   // namespace

    /**
     * @brief Get open position drafts
     *
     * @param positionStore
     * @param stockStore
     * @param transactionStore
     * @return std::vector<drafts::PositionDraft>
     */
    std::vector<drafts::PositionDraft> getOpenPositionDrafts(
        const app::PositionStore&    positionStore,
        const app::StockStore&       stockStore,
        const app::TransactionStore& transactionStore
    )
    {
        const auto positions = positionStore.getOpenPositions();
        return _getPositionDrafts(positions, stockStore, transactionStore);
    }

    /**
     * @brief Get open position drafts for a specific account
     *
     * @param account
     * @param positionStore
     * @param stockStore
     * @param transactionStore
     * @return std::vector<drafts::PositionDraft>
     */
    std::vector<drafts::PositionDraft> getOpenPositionDrafts(
        AccountId                    account,
        const app::PositionStore&    positionStore,
        const app::StockStore&       stockStore,
        const app::TransactionStore& transactionStore
    )
    {
        const auto positions = positionStore.getOpenPositions({account});
        return _getPositionDrafts(positions, stockStore, transactionStore);
    }

}   // namespace controller
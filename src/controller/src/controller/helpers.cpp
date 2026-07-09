#include "helpers.hpp"

#include <expected>

#include "controller/mapper/stock_mapper.hpp"
#include "drafts/position_draft.hpp"
#include "drafts/transaction/transaction_create_draft.hpp"
#include "finance/position.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "logging/log_macros.hpp"
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

    /**
     * @brief Get open position drafts for a specific account
     *
     * @param account
     * @param positionStore
     * @param stockStore
     * @param transactionStore
     * @return std::vector<drafts::PositionDraft>
     */
    std::vector<OpenStockPositionDetail> getOpenStockPositionDetails(
        AccountId                                        account,
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<store::IStockStore>&       stockStore,
        const std::shared_ptr<store::ITransactionStore>& transactionStore
    )
    {
        const auto positions = positionStore->getOpenPositions();
        auto       filter    = finance::TransactionFilter();
        filter.setPositionIds(positions.getIds());
        auto positionTxs = transactionStore->getStockPositions(filter);

        positionTxs.erase_if(
            [&](const auto& pair)
            { return pair.second.getSecurityAccount() != account; }
        );

        std::vector<OpenStockPositionDetail> drafts;

        for (auto& [id, txs] : positionTxs)
        {
            if (!positions.contains(id))
            {
                LOG_ERROR(
                    "No position found for position id: " + id.toString()
                );
                continue;
            }

            const auto position = positions.at(id);

            if (txs.empty())
            {
                LOG_ERROR(
                    "No transactions found for position id: " +
                    position.getId().toString()
                );
                continue;
            }

            const auto instrumentId = txs.getBaseInstrument();

            const auto& stock = stockStore->getStock(instrumentId);

            if (!stock)
            {
                LOG_ERROR(
                    "No stock found for instrument id: " +
                    position.getId().toString()
                );
                continue;
            }

            const auto stockInfo = StockMapper::toStockInfoDraft(stock.value());

            drafts.emplace_back(
                OpenStockPositionDetail{
                    .positionDraft =
                        drafts::PositionStockDetailDraft{
                            position.getId(),
                            stockInfo,
                            position.getCreatedAt(),
                            txs.getPnL()->getQuantity(),
                            txs.getPnL()->getAverageCost(),
                            txs.getPnL()->getCostBasis(),
                            txs.getPnL()->getRealizedPnL(),
                            txs.getPnL()->getRealizedPnLPercentage()
                        },
                    .ticker = stockInfo.getTicker(),
                    .pnl    = txs.getPnL()
                }
            );
        }

        return drafts;
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
    std::vector<drafts::PositionStockDetailDraft> getOpenStockPositions(
        AccountId                                        account,
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<store::IStockStore>&       stockStore,
        const std::shared_ptr<store::ITransactionStore>& transactionStore
    )
    {
        const auto details = getOpenStockPositionDetails(
            account,
            positionStore,
            stockStore,
            transactionStore
        );

        std::vector<drafts::PositionStockDetailDraft> drafts;
        drafts.reserve(details.size());
        for (const auto& detail : details)
            drafts.push_back(detail.positionDraft);

        return drafts;
    }

}   // namespace controller
#include "helpers.hpp"

#include "cache/account_cache.hpp"
#include "cache/stock_cache.hpp"
#include "controller/mapper/stock_mapper.hpp"
#include "drafts/position_draft.hpp"
#include "finance/position.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "logging/log_macros.hpp"
#include "store/i_position_store.hpp"
#include "store/i_transaction_store.hpp"

REGISTER_LOG_CATEGORY("Controller.Helpers");

namespace controller
{
    /**
     * @brief Get open position drafts for a specific account
     *
     * @param account
     * @param positionStore
     * @param stockCache
     * @param transactionStore
     * @return std::vector<drafts::PositionDraft>
     */
    std::vector<OpenStockPositionDetail> getOpenStockPositionDetails(
        AccountId                                        account,
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<cache::StockCache>&        stockCache,
        const std::shared_ptr<cache::AccountCache>&      accountCache,
        const std::shared_ptr<store::ITransactionStore>& transactionStore
    )
    {
        const auto positions = positionStore->getOpenPositions({account});
        auto       filter    = finance::TransactionFilter();
        filter.setPositionIds(positions.getIds());
        auto positionTxs = transactionStore->getStockPositions(
            filter,
            accountCache->getAllAccounts()
        );

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

            const auto& stock = stockCache->getStock(instrumentId);

            if (stock == nullptr)
            {
                LOG_ERROR(
                    "No stock found for instrument id: " +
                    position.getId().toString()
                );
                continue;
            }

            const auto stockInfo = StockMapper::toStockInfoDraft(*stock);

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
     * @param stockCache
     * @param transactionStore
     * @return std::vector<drafts::PositionDraft>
     */
    std::vector<drafts::PositionStockDetailDraft> getOpenStockPositions(
        AccountId                                        account,
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<cache::StockCache>&        stockCache,
        const std::shared_ptr<cache::AccountCache>&      accountCache,
        const std::shared_ptr<store::ITransactionStore>& transactionStore
    )
    {
        const auto details = getOpenStockPositionDetails(
            account,
            positionStore,
            stockCache,
            accountCache,
            transactionStore
        );

        std::vector<drafts::PositionStockDetailDraft> drafts;
        drafts.reserve(details.size());
        for (const auto& detail : details)
            drafts.push_back(detail.positionDraft);

        return drafts;
    }

}   // namespace controller
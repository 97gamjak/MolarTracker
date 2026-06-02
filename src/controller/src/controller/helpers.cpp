#include "helpers.hpp"

#include <expected>

#include "controller/mapper/stock_mapper.hpp"
#include "drafts/position_draft.hpp"
#include "drafts/stock_draft.hpp"
#include "drafts/transaction/transaction_create_draft.hpp"
#include "finance/position.hpp"
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

    namespace
    {
        /**
         * @brief Get stock info draft by position
         *
         * @param position
         * @param stockStore
         * @param transactionStore
         * @return std::optional<drafts::StockInfoDraft>
         */
        std::optional<drafts::StockInfoDraft> _getStockInfoByPosition(
            const finance::Position&                         position,
            const std::shared_ptr<store::IStockStore>&       stockStore,
            const std::shared_ptr<store::ITransactionStore>& transactionStore
        )
        {
            const auto ids =
                transactionStore->getInstrumentIdsByPositionId(position.getId()
                );

            const auto& stocks = stockStore->getStocks(ids);

            if (stocks.empty())
            {
                LOG_WARNING(
                    "No stock found for instrument id: " +
                    position.getId().toString()
                );
                return std::nullopt;
            }

            return StockMapper::toStockInfoDraft(stocks.front());
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
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<store::IStockStore>&       stockStore,
        const std::shared_ptr<store::ITransactionStore>& transactionStore
    )
    {
        const auto positions = positionStore->getOpenPositions();

        std::vector<drafts::PositionDraft> drafts;
        for (const auto& position : positions)
        {
            const auto& stock =
                _getStockInfoByPosition(position, stockStore, transactionStore);

            if (!stock)
                continue;

            drafts.emplace_back(
                position.getId(),
                stock.value(),
                position.getCreatedAt()
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
    std::vector<drafts::PositionStockDetailDraft> getOpenStockPositionDrafts(
        AccountId                                        account,
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<store::IStockStore>&       stockStore,
        const std::shared_ptr<store::ITransactionStore>& transactionStore
    )
    {
        const auto positions = positionStore->getOpenPositions({account});

        std::vector<drafts::PositionStockDetailDraft> drafts;

        for (const auto& position : positions)
        {
            const auto txs =
                transactionStore->findTransactionsByPositionId(position.getId())
                    .stocks();

            if (txs.empty())
            {
                LOG_ERROR(
                    "No transactions found for position id: " +
                    position.getId().toString()
                );
                continue;
            }

            const auto instrumentIds = txs.getBaseInstrumentIds();

            const auto& stocks = stockStore->getStocks(instrumentIds);

            if (stocks.empty())
            {
                LOG_ERROR(
                    "No stock found for instrument id: " +
                    position.getId().toString()
                );
                continue;
            }

            const auto stockInfo =
                StockMapper::toStockInfoDraft(stocks.front());

            drafts.emplace_back(
                position.getId(),
                stockInfo,
                position.getCreatedAt(),
                txs.getTotalQuantity()
            );
        }

        return drafts;
    }

}   // namespace controller
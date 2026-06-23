#include "helpers.hpp"

#include <expected>

#include "config/id_types.hpp"
#include "controller/mapper/stock_mapper.hpp"
#include "drafts/position/position_stock_draft.hpp"
#include "drafts/transaction/transaction_create_draft.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "gateway/position_gateway.hpp"
#include "logging/log_macros.hpp"
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
        const std::shared_ptr<gateway::PositionGateway>& positionGateway,
        const std::shared_ptr<store::IStockStore>&       stockStore
    )
    {
        const auto positions =
            positionGateway->getOpenPositionTransactions({account})
                .getStockPositions();

        std::vector<OpenStockPositionDetail> drafts;

        for (auto position : positions)
        {
            const auto instrumentId = position.getBaseInstrument();

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
                            position.getPosition().getCreatedAt(),
                            position.getPnL()->getQuantity(),
                            position.getPnL()->getAverageCost(),
                            position.getPnL()->getCostBasis(),
                            position.getPnL()->getRealizedPnL(),
                            position.getPnL()->getRealizedPnLPercentage()
                        },
                    .ticker = stockInfo.getTicker(),
                    .pnl    = position.getPnL()
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
        const std::shared_ptr<gateway::PositionGateway>& positionGateway,
        const std::shared_ptr<store::IStockStore>&       stockStore
    )
    {
        const auto details =
            getOpenStockPositionDetails(account, positionGateway, stockStore);

        std::vector<drafts::PositionStockDetailDraft> drafts;
        drafts.reserve(details.size());
        for (const auto& detail : details)
            drafts.push_back(detail.positionDraft);

        return drafts;
    }

    std::vector<OpenOptionPositionDetail> getOpenOptionPositionDetails(
        AccountId                                        account,
        const std::shared_ptr<gateway::PositionGateway>& positionGateway,
        const std::shared_ptr<store::IOptionStore>&      optionStore
    )
    {
        const auto positions =
            positionGateway->getOpenPositionTransactions({account})
                .getOptionPositions();

        std::vector<OpenOptionPositionDetail> drafts;

        for (auto position : positions)
        {
            const auto instrumentId = position.getBaseInstrument();

            // TODO: implement fast path for retrieving option details based on
            // single instrument id lookup
            const auto& options = optionStore->getOptions({instrumentId});

            if (options.empty() || options.size() > 1)
            {
                LOG_ERROR(
                    "No option found for instrument id: " +
                    position.getId().toString()
                );
                continue;
            }

            const auto stock = options.getValues()[0].getUnderlying();

            const auto stockInfo = StockMapper::toStockInfoDraft(stock);

            drafts.emplace_back(
                OpenOptionPositionDetail{
                    .positionDraft =
                        drafts::PositionOptionDetailDraft{
                            position.getId(),
                            stockInfo,
                            position.getPosition().getCreatedAt(),
                            position.getPnL()->getQuantity(),
                            position.getPnL()->getRealizedPnL(),
                            position.getPnL()->getRealizedPnLPercentage()
                        },
                    .ticker = stockInfo.getTicker(),
                    .pnl    = position.getPnL()
                }
            );
        }

        return drafts;
    }

}   // namespace controller
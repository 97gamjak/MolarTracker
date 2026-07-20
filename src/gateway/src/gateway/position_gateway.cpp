#include "gateway/position_gateway.hpp"

#include "error/finance_error.hpp"
#include "finance/positions.hpp"
#include "finance/transaction/pnl.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "logging/log_macros.hpp"
#include "mapper/stock_mapper.hpp"
#include "store/i_option_store.hpp"
#include "store/i_position_store.hpp"
#include "store/i_stock_store.hpp"
#include "store/i_transaction_store.hpp"

REGISTER_LOG_CATEGORY("Gateway.PositionGateway");

namespace gateway
{

    namespace
    {
        finance::TransactionFilter _getOpenPositionsFilter(
            const finance::Positions& positions
        )
        {
            finance::TransactionFilter filter;
            filter.positionIds = positions.getOpenPositions().getIds();

            return filter;
        }

        FinanceResult<finance::PositionEvents> _getPositionEvents(
            const finance::Transactions&                txs,
            const std::shared_ptr<store::IOptionStore>& optionStore
        )
        {
            finance::PositionEvents events;
            events.reserve(txs.stocks().size() + txs.options().size());

            for (const auto& tx : txs.stocks())
            {
                events.add(
                    finance::PositionEvent{
                        .timestamp = tx.getTimestamp(),
                        .data =
                            finance::StockTrade{
                                .quantity  = tx.getQuantity(),
                                .unitPrice = tx.getUnitPrice(),
                                .fees      = tx.getFees()
                            }
                    }
                );
            }

            for (const auto& tx : txs.options())
            {
                const auto option =
                    optionStore->getOption(tx.getInstrumentId());
                if (!option)
                {
                    return FinanceError{
                        FinanceErrorType::UnknownOption,
                        std::format(
                            "Option {} not found while building PnL events",
                            tx.getInstrumentId().toString()
                        )
                    };
                }

                events.add(
                    finance::PositionEvent{
                        .timestamp = tx.getTimestamp(),
                        .data =
                            finance::OptionTrade{
                                .type         = option->getOptionType(),
                                .buySell      = tx.getBuySell(),
                                .action       = tx.getAction(),
                                .strike       = option->getStrikePrice(),
                                .quantity     = tx.getQuantity(),
                                .contractSize = option->getContractSize(),
                                .premium      = tx.getPremium(),
                                .fees         = tx.getFees()
                            }
                    }
                );
            }

            events.sort();
            return events;
        }
    }   // namespace

    PositionGateway::PositionGateway(
        const std::shared_ptr<store::ITransactionStore>& transactionStore,
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<store::IOptionStore>&      optionStore,
        const std::shared_ptr<store::IStockStore>&       stockStore
    )
        : _transactionStore(transactionStore),
          _positionStore(positionStore),
          _optionStore(optionStore),
          _stockStore(stockStore)
    {
    }

    FinanceResult<std::vector<std::pair<finance::Position, finance::Transactions>>> PositionGateway::
        getOpenPositionTransactions(const IdSet<AccountId>& accountIds) const
    {
        const auto positions = _positionStore->getOpenPositions();

        auto filter       = _getOpenPositionsFilter(positions);
        filter.accountIds = accountIds;

        auto txsResult = _transactionStore->getTransactions(filter);

        if (!txsResult)
            return txsResult.error();

        std::vector<std::pair<finance::Position, finance::Transactions>> result;
        for (const auto& [positionId, tx] : txsResult.value().groupByPosition())
        {
            const auto position = positions.at(positionId);

            result.emplace_back(position, tx);
        }

        return result;
    }

    PnLResult<finance::PositionPnl> PositionGateway::calculatePositionPnl(
        const finance::Transactions& positionTxs,
        std::optional<Cash>          markPrice
    ) const
    {
        auto eventsResult = _getPositionEvents(positionTxs, _optionStore);
        if (!eventsResult)
        {
            return FromError<FinanceError, PnLError>::apply(
                eventsResult.error(),
                PnLErrorType::UnknownOption
            );
        }

        auto stateResult =
            foldEvents(finance::PositionState{}, eventsResult.value());

        if (!stateResult)
            return stateResult.error();

        return snapshot(stateResult.value(), markPrice);
    }

    /**
     * @brief Get open position drafts for a specific account
     *
     * @param account
     * @param positionStore
     * @param stockStore
     * @param transactionStore
     * @return FinanceResult<std::vector<OpenStockPositionDetail>>
     */
    FinanceResult<std::vector<OpenStockPositionDetail>> PositionGateway::
        getOpenStockPositionDetails(AccountId account) const
    {
        const auto positions = getOpenPositionTransactions({account});

        if (!positions)
            return positions.error();

        std::vector<OpenStockPositionDetail> drafts;

        for (const auto& [position, positionTransaction] : positions.value())
        {
            if (positionTransaction.containsOptions())
                continue;

            const auto instrumentIds =
                positionTransaction.getStockInstrumentIds();

            if (instrumentIds.size() != 1)
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidPosition,
                    std::format(
                        "Position {} has {} stock instrument ids",
                        position.getId().toString(),
                        instrumentIds.size()
                    )
                };
                LOG_ERROR(error.toString());
                return error;
            }

            const auto& stock = _stockStore->getStock(instrumentIds.front());

            if (!stock)
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidStock,
                    std::format(
                        "No stock found for instrument id: {}",
                        instrumentIds.front().toString()
                    )
                };
                LOG_ERROR(error.toString());
                return error;
            }

            auto eventsResult =
                _getPositionEvents(positionTransaction, _optionStore);
            if (!eventsResult)
            {
                LOG_ERROR(eventsResult.error().toString());
                return eventsResult.error();
            }

            auto stateResult = finance::foldEvents(
                finance::PositionState{},
                eventsResult.value()
            );
            if (!stateResult)
            {
                LOG_ERROR(stateResult.error().toString());
                return FromError<PnLError, FinanceError>::apply(
                    stateResult.error(),
                    FinanceErrorType::PnlError
                );
            }

            const auto initialPnl =
                finance::snapshot(stateResult.value(), std::nullopt);
            const auto stockInfo =
                mapper::StockMapper::toStockInfoDraft(stock.value());

            drafts.emplace_back(
                OpenStockPositionDetail{
                    .positionDraft =
                        drafts::PositionStockDetailDraft{
                            position.getId(),
                            stockInfo,
                            position.getCreatedAt(),
                            initialPnl.quantity,
                            initialPnl.getAverageCost(),
                            initialPnl.costBasis,
                            initialPnl.realizedPnL,
                            initialPnl.getRealizedPnLPercentage()
                        },
                    .ticker = stockInfo.getTicker(),
                    .state  = stateResult.value()
                }
            );
        }

        return drafts;
    }

    FinanceResult<std::vector<drafts::PositionStockDetailDraft>> PositionGateway::
        getOpenStockPosition(AccountId account) const
    {
        const auto positions = getOpenPositionTransactions({account});

        if (!positions)
            return positions.error();

        std::vector<drafts::PositionStockDetailDraft> drafts;

        for (const auto& [position, positionTransaction] : positions.value())
        {
            if (positionTransaction.containsOptions())
                continue;

            const auto instrumentIds =
                positionTransaction.getStockInstrumentIds();

            if (instrumentIds.size() != 1)
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidPosition,
                    std::format(
                        "Position {} has {} stock instrument ids",
                        position.getId().toString(),
                        instrumentIds.size()
                    )
                };
                LOG_ERROR(error.toString());
                return error;
            }

            const auto& stock = _stockStore->getStock(instrumentIds.front());

            if (!stock)
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidStock,
                    std::format(
                        "No stock found for instrument id: {}",
                        instrumentIds.front().toString()
                    )
                };
                LOG_ERROR(error.toString());
                return error;
            }

            auto pnlResult = calculatePositionPnl(
                positionTransaction,
                std::nullopt   // TODO: no mark price provided
            );
            if (!pnlResult)
            {
                const auto error = FromError<PnLError, FinanceError>::apply(
                    pnlResult.error(),
                    FinanceErrorType::PnlError
                );
                LOG_ERROR(error.toString());
                return error;
            }

            const auto& pnl = pnlResult.value();

            const auto stockInfo =
                mapper::StockMapper::toStockInfoDraft(stock.value());

            drafts.emplace_back(
                position.getId(),
                stockInfo,
                position.getCreatedAt(),
                pnl.quantity,
                pnl.getAverageCost(),
                pnl.costBasis,
                pnl.realizedPnL,
                pnl.getRealizedPnLPercentage()

            );
        }
        return drafts;
    }

    FinanceResult<std::vector<OpenOptionPositionDetail>> PositionGateway::
        getOpenOptionPositionDetails(AccountId account) const
    {
        const auto positions = getOpenPositionTransactions({account});

        if (!positions)
            return positions.error();

        std::vector<OpenOptionPositionDetail> drafts;

        for (const auto& [position, positionTransaction] : positions.value())
        {
            if (!positionTransaction.containsOptions())
                continue;

            const auto instrumentIds =
                positionTransaction.getOptionInstrumentIds();

            if (instrumentIds.size() != 1)
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidPosition,
                    std::format(
                        "Position {} has {} option instrument ids",
                        position.getId().toString(),
                        instrumentIds.size()
                    )
                };
                LOG_ERROR(error.toString());
                return error;
            }

            const auto option = _optionStore->getOption(instrumentIds.front());

            if (!option)
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidOption,
                    std::format(
                        "No option found for instrument id: {}",
                        instrumentIds.front().toString()
                    )
                };
                LOG_ERROR(error.toString());
                return error;
            }

            const auto& stock = option->getUnderlying();

            auto eventsResult =
                _getPositionEvents(positionTransaction, _optionStore);
            if (!eventsResult)
            {
                LOG_ERROR(eventsResult.error().toString());
                return eventsResult.error();
            }

            auto stateResult = finance::foldEvents(
                finance::PositionState{},
                eventsResult.value()
            );
            if (!stateResult)
            {
                LOG_ERROR(stateResult.error().toString());
                return FromError<PnLError, FinanceError>::apply(
                    stateResult.error(),
                    FinanceErrorType::PnlError
                );
            }

            const auto initialPnl =
                finance::snapshot(stateResult.value(), std::nullopt);

            const auto stockInfo = mapper::StockMapper::toStockInfoDraft(stock);

            drafts.emplace_back(
                OpenOptionPositionDetail{
                    .positionDraft =
                        drafts::PositionOptionDetailDraft{
                            position.getId(),
                            stockInfo,
                            position.getCreatedAt(),
                            initialPnl.quantity,
                            initialPnl.realizedPnL,
                            initialPnl.getRealizedPnLPercentage()
                        },
                    .ticker = stockInfo.getTicker(),
                    .state  = stateResult.value()
                }
            );
        }

        return drafts;
    }

}   // namespace gateway
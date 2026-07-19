#include "gateway/position_gateway.hpp"

#include "finance/transaction/pnl.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "logging/log_macros.hpp"
#include "store/i_option_store.hpp"
#include "store/i_position_store.hpp"
#include "store/i_transaction_store.hpp"

REGISTER_LOG_CATEGORY("Gateway.PositionGateway");

namespace gateway
{

    namespace
    {
        finance::TransactionFilter _getOpenPositionsFilter(
            const std::shared_ptr<store::IPositionStore>& positionStore
        )
        {
            const auto openPositions = positionStore->getOpenPositions();

            finance::TransactionFilter filter;
            filter.positionIds = openPositions.getIds();

            return filter;
        }
    }   // namespace

    PositionGateway::PositionGateway(
        const std::shared_ptr<store::ITransactionStore>& transactionStore,
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<store::IOptionStore>&      optionStore
    )
        : _transactionStore(transactionStore),
          _positionStore(positionStore),
          _optionStore(optionStore)
    {
    }

    FinanceResult<IdMap<PositionId, finance::Transactions>> PositionGateway::
        getOpenPositionTransactions(const IdSet<AccountId>& accountIds) const
    {
        auto filter       = _getOpenPositionsFilter(_positionStore);
        filter.accountIds = accountIds;

        auto txsResult = _transactionStore->getTransactions(filter);

        if (!txsResult)
            return txsResult.error();

        return txsResult.value().groupByPosition();
    }

    FinanceResult<finance::PositionEvents> PositionGateway::getPositionEvents(
        const finance::Transactions&                txs,
        const std::shared_ptr<store::IOptionStore>& optionStore
    ) const
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
            const auto option = optionStore->getOption(tx.getInstrumentId());
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

}   // namespace gateway
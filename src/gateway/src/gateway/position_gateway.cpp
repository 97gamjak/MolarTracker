#include "gateway/position_gateway.hpp"

#include "finance/transaction/transaction_filter.hpp"
#include "logging/log_macros.hpp"
#include "store/i_option_store.hpp"
#include "store/i_position_store.hpp"
#include "store/i_transaction_store.hpp"

REGISTER_LOG_CATEGORY("Gateway.PositionGateway");

namespace gateway
{

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

    FinanceResult<finance::PositionTransactions> PositionGateway::
        getOpenPositionTransactions(const IdSet<AccountId>& accountIds) const
    {
        const auto [positions, filter] = _getOpenPositionsFilter();

        auto txsResult = _transactionStore->getTransactions(filter);

        if (!txsResult)
            return txsResult.error();

        auto txs = txsResult.value();

        const auto options = _getNeededOptions(txs);

        const auto populateResult = txs.populateOptions(options);
        if (!populateResult)
        {
            const auto error = populateResult.error().convert(
                FinanceErrorType::InvalidTransaction,
                "Failed to populate options for transactions"
            );
            LOG_ERROR(error.toString());
            return error;
        }

        return finance::PositionTransactions::fromTransactions(
            txs.filter(accountIds),
            positions
        );
    }

    std::pair<finance::Positions, finance::TransactionFilter> PositionGateway::
        _getOpenPositionsFilter() const
    {
        const auto openPositions = _positionStore->getOpenPositions();

        finance::TransactionFilter filter;
        filter.setPositionIds(openPositions.getIds());

        return {openPositions, filter};
    }

    finance::Options PositionGateway::_getNeededOptions(
        const finance::Transactions& txs
    ) const
    {
        finance::Options options;

        const auto neededOptions = txs.getNeededOptionPopulation();
        return _optionStore->getOptions(neededOptions);
    }

}   // namespace gateway
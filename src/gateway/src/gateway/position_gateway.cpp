#include "gateway/position_gateway.hpp"

#include <stdexcept>

#include "finance/transaction/transaction_filter.hpp"
#include "store/i_account_store.hpp"
#include "store/i_option_store.hpp"
#include "store/i_position_store.hpp"
#include "store/i_transaction_store.hpp"

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

    finance::PositionTransactions PositionGateway::getOpenPositionTransactions(
        const IdSet<AccountId>& accountIds
    ) const
    {
        const auto [positions, filter] = _getOpenPositionsFilter();

        auto txs = _transactionStore->getTransactions(filter);

        const auto options = _getNeededOptions(txs);

        if (!txs.populateOptions(options))
        {
            throw std::runtime_error(
                "Failed to populate options for open position transactions"
            );
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
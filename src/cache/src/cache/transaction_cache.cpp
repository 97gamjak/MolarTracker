#include "cache/transaction_cache.hpp"

#include "finance/account/accounts.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "finance/transaction/transactions.hpp"
#include "store/i_transaction_store.hpp"

namespace cache
{
    TransactionCache::TransactionCache(
        const std::shared_ptr<store::ITransactionStoreReader>& reader
    )
        : _reader(reader)
    {
    }

    std::shared_ptr<const finance::DomainTransaction> TransactionCache::_load(
        const TransactionId& key
    )
    {
        const auto transaction = _reader->getTransaction(key);

        if (transaction)
            return std::make_shared<const finance::DomainTransaction>(
                transaction.value()
            );

        return nullptr;
    }

    finance::TransactionsView TransactionCache::getTransactions(
        const finance::AccountsView& accounts
    )
    {
        IdSet<AccountId> accountIds = accounts.getIds() - _accountIds;

        const auto transactions =
            _reader->getTransactions(finance::TransactionFilter(), accountIds);

        _accountIds &= accountIds;

        for (const auto& transaction : transactions)
        {
            _addAndNotify(
                transaction.getId(),
                std::make_shared<const finance::DomainTransaction>(transaction)
            );
        }

        return finance::TransactionsView(_getEntries().getValues(), accounts);
    }

}   // namespace cache
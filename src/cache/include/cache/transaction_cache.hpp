#ifndef __CACHE__INCLUDE__CACHE__TRANSACTION_CACHE_HPP__
#define __CACHE__INCLUDE__CACHE__TRANSACTION_CACHE_HPP__

#include "cache/single_cache.hpp"
#include "config/id_types.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/transactions.hpp"

namespace store
{
    class ITransactionStoreReader;   // forward declaration
}   // namespace store

namespace finance
{
    class AccountsView;   // forward declaration
}   // namespace finance

namespace cache
{
    class TransactionCache
        : public SingleCache<TransactionId, finance::DomainTransaction>
    {
       private:
        std::shared_ptr<store::ITransactionStoreReader> _reader;

        IdSet<AccountId> _accountIds;

       public:
        explicit TransactionCache(
            const std::shared_ptr<store::ITransactionStoreReader>& reader
        );

        [[nodiscard]]
        finance::TransactionsView getTransactions(
            const finance::AccountsView& accounts
        );

       protected:
        [[nodiscard]]
        std::shared_ptr<const finance::DomainTransaction> _load(
            const TransactionId& key
        ) override;
    };

}   // namespace cache

#endif   // __CACHE__INCLUDE__CACHE__TRANSACTION_CACHE_HPP__
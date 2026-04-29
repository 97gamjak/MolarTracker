#ifndef __APP__INCLUDE__APP__STORE__TRANSACTION_STORE_HPP__
#define __APP__INCLUDE__APP__STORE__TRANSACTION_STORE_HPP__

#include <memory>
#include <unordered_map>
#include <vector>

#include "app/services_api/i_transaction_service.hpp"
#include "base/base_store.hpp"
#include "config/id_types.hpp"
#include "drafts/transaction_draft.hpp"
#include "finance/transaction.hpp"

namespace app
{
    /**
     * @brief Result of transaction store operations
     *
     */
    enum class TransactionStoreResult : std::uint8_t
    {
        Ok,
        Error,
        TransactionSumNotZero,
    };

    /**
     * @brief Store for managing transactions
     *
     */
    class TransactionStore
        : public BaseStore<finance::Transaction, TransactionId>
    {
       private:
        /// The Transaction service
        std::shared_ptr<ITransactionService> _transactionService;

       public:
        explicit TransactionStore(
            const std::shared_ptr<ITransactionService>& transactionService
        );

        void commit(
            const std::unordered_map<AccountId, AccountId, AccountId::Hash>&
                accountIdMap
        );

        TransactionStoreResult addTransaction(
            finance::Transaction& transaction
        );

        // TODO (97gamjak): create filter here
        std::vector<finance::Transaction> getTransactions() const;

       private:
        void _updateAccountIds(
            const std::unordered_map<AccountId, AccountId, AccountId::Hash>&
                accountIdMap
        );
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__TRANSACTION_STORE_HPP__
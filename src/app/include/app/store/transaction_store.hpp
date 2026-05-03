#ifndef __APP__INCLUDE__APP__STORE__TRANSACTION_STORE_HPP__
#define __APP__INCLUDE__APP__STORE__TRANSACTION_STORE_HPP__

#include <memory>
#include <vector>

#include "app/store/account_store.hpp"
#include "base/base_store.hpp"
#include "config/id_types.hpp"
#include "finance/transaction.hpp"

namespace app
{
    class ITransactionService;   // Forward declaration
    class AccountStore;          // Forward declaration

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

        /// Connection for account ID remapping
        Connection _accountIdRemapConnection;

       public:
        explicit TransactionStore(
            const std::shared_ptr<ITransactionService>& transactionService,
            const AccountStore&                         accountStore
        );

        void commit();

        TransactionStoreResult addTransaction(finance::Transaction transaction);

        // TODO (97gamjak): create filter here
        std::vector<finance::Transaction> getTransactions() const;

       private:
        void _onAccountIdRemap(const AccountStore::IdMap& remap);
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__TRANSACTION_STORE_HPP__
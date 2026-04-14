#ifndef __APP__INCLUDE__APP__STORE__TRANSACTION_STORE_HPP__
#define __APP__INCLUDE__APP__STORE__TRANSACTION_STORE_HPP__

#include <memory>

#include "app/services_api/i_transaction_service.hpp"
#include "base/base_store.hpp"
#include "config/id_types.hpp"
#include "finance/transaction.hpp"

namespace app
{
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

        void commit() override;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__TRANSACTION_STORE_HPP__
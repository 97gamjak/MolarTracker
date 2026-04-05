#ifndef __APP__INCLUDE__APP__SERVICES_API__I_TRANSACTION_SERVICE_HPP__
#define __APP__INCLUDE__APP__SERVICES_API__I_TRANSACTION_SERVICE_HPP__

#include "finance/transaction.hpp"

namespace app
{
    /**
     * @brief Interface for transaction service
     *
     */
    class ITransactionService
    {
       public:
        virtual ~ITransactionService() = default;

        /**
         * @brief Adds a transaction to the service.
         *
         * @param transaction The transaction to add.
         *
         * @return The ID of the added transaction.
         */
        virtual TransactionId addTransaction(
            const finance::Transaction& transaction
        ) = 0;
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__SERVICES_API__I_TRANSACTION_SERVICE_HPP__
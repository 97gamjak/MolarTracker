#ifndef __APP__SERVICES_API__I_TRANSACTION_SERVICE_HPP__
#define __APP__SERVICES_API__I_TRANSACTION_SERVICE_HPP__

#include "finance/transaction.hpp"

namespace app
{
    class ITransactionService
    {
       public:
        virtual ~ITransactionService() = default;

        virtual TransactionId addTransaction(
            const finance::Transaction& transaction
        ) = 0;
    };
}   // namespace app

#endif   // __APP__SERVICES_API__I_TRANSACTION_SERVICE_HPP__
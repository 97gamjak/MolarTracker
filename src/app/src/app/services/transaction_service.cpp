#include "transaction_service.hpp"

namespace app
{
    TransactionService::TransactionService(
        std::shared_ptr<ITransactionRepo> transactionRepo
    )
        : _transactionRepo(std::move(transactionRepo))
    {
    }

    [[nodiscard]] TransactionId TransactionService::addTransaction(
        const finance::Transaction& transaction
    )
    {
        return _transactionRepo->addTransaction(transaction);
    }
}   // namespace app

#include "app/store/transaction_store.hpp"

namespace app
{
    /**
     * @brief Construct a new Transaction Store object
     *
     * @param transactionService
     */
    TransactionStore::TransactionStore(
        const std::shared_ptr<ITransactionService>& transactionService
    )
        : _transactionService(transactionService)
    {
    }

    /**
     * @brief Save all temporary changes to the database
     *
     */
    void TransactionStore::commit()
    {
        // TODO(97gamjak): Implementation of commit logic
    }

}   // namespace app

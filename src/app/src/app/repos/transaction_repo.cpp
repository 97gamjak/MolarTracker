#include "transaction_repo.hpp"

#include "app/factories/transaction_factory.hpp"
#include "config/id_types.hpp"
#include "db/transaction.hpp"
#include "orm/crud.hpp"
#include "repo_errors.hpp"

namespace app
{
    void TransactionRepo::addTransaction(
        const finance::Transaction& transaction
    )
    {
        db::Transaction dbTransaction{_getDb()};

        const auto transactionResult = orm::Crud().insert(
            _getDb(),
            TransactionFactory::toTransactionRow(transaction)
        );

        if (!transactionResult.has_value())
        {
            const auto msg =
                getInsertError(transactionResult.error(), "transaction");

            LOG_ERROR(msg);
            throw orm::CrudException(msg);
        }

        for (const auto& entry : transaction.getEntries())
        {
            const auto instrumentResult = orm::Crud().insert(
                _getDb(),
                dbTransaction,
                TransactionFactory::toInstrumentRow(entry.getDetails())
            );

            if (!instrumentResult.has_value())
            {
                const auto msg =
                    getInsertError(instrumentResult.error(), "instrument");

                LOG_ERROR(msg);
                throw orm::CrudException(msg);
            }

            const auto entryRow = TransactionFactory::toTransactionEntryRow(
                entry,
                TransactionId(transactionResult.value()),
                InstrumentId(instrumentResult.value())
            );

            const auto entryResult =
                orm::Crud().insert(_getDb(), dbTransaction, entryRow);

            if (!entryResult.has_value())
            {
                const auto msg =
                    getInsertError(entryResult.error(), "transaction entry");

                LOG_ERROR(msg);
                throw orm::CrudException(msg);
            }
        }

        dbTransaction.commit();
    }

}   // namespace app
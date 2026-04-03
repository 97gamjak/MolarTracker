#include "transaction_repo.hpp"

#include "app/factories/transaction_factory.hpp"
#include "config/id_types.hpp"
#include "db/transaction.hpp"
#include "finance/transaction.hpp"
#include "orm/crud.hpp"
#include "repo_errors.hpp"

namespace app
{
    TransactionId TransactionRepo::addTransaction(
        const finance::Transaction& transaction
    )
    {
        db::Transaction dbTx{_getDb()};
        auto txRow = TransactionFactory::toTransactionRow(transaction);

        const auto transactionResult = orm::Crud().insert(_getDb(), txRow);

        if (!transactionResult.has_value())
        {
            const auto msg =
                getInsertError(transactionResult.error(), "transaction");

            LOG_ERROR(msg);
            throw orm::CrudException(msg);
        }

        const auto txId = TransactionId(transactionResult.value());

        for (const auto& entry : transaction.getEntries())
        {
            const auto instrumentResult = orm::Crud().insert(
                _getDb(),
                dbTx,
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
                txId,
                InstrumentId(instrumentResult.value())
            );

            const auto entryResult =
                orm::Crud().insert(_getDb(), dbTx, entryRow);

            if (!entryResult.has_value())
            {
                const auto msg =
                    getInsertError(entryResult.error(), "transaction entry");

                LOG_ERROR(msg);
                throw orm::CrudException(msg);
            }
        }

        dbTx.commit();

        return TransactionId(transactionResult.value());
    }

}   // namespace app
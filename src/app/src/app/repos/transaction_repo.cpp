#include "transaction_repo.hpp"

#include "app/factories/transaction_factory.hpp"
#include "app/repos_api/i_instrument_repo.hpp"
#include "config/id_types.hpp"
#include "db/database.hpp"
#include "db/transaction.hpp"
#include "finance/transaction.hpp"
#include "orm/crud.hpp"
#include "orm/join.hpp"
#include "repo_errors.hpp"
#include "sql_models/instrument_row.hpp"

namespace app
{
    TransactionRepo::TransactionRepo(
        db::Database&                    db,
        std::shared_ptr<IInstrumentRepo> instrumentRepo
    )
        : BaseRepo(db), _instrumentRepo(std::move(instrumentRepo))
    {
    }

    /**
     * @brief add a transaction to the database
     *
     * @param transaction
     * @return TransactionId
     */
    TransactionId TransactionRepo::addTransaction(
        const finance::Transaction& transaction
    )
    {
        db::Transaction dbTx{_getDb()};
        auto            txRow = TransactionFactory::toRow(transaction);

        const auto transactionResult = _getCrud().insert(_getDb(), txRow);

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
            // 1. check if instrument exists -> if not create it
            const auto instrument =
                TransactionFactory::toInstrumentRow(entry.getDetails());

            const auto idOpt = _instrumentRepo->getInstrumentId(instrument);

            if (!idOpt.has_value())
            {
                throw std::runtime_error("Instrument not found");
            }

            const auto entryRow =
                TransactionFactory::toEntryRow(entry, txId, idOpt.value());

            const auto entryResult =
                _getCrud().insert(_getDb(), dbTx, entryRow);

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

    /**
     * @brief get all transactions from the database
     *
     * @return std::vector<finance::Transaction>
     */
    std::vector<finance::Transaction> TransactionRepo::getTransactions()
    {
        const auto txRows = _getCrud().get<TransactionRow>(_getDb());

        std::vector<finance::Transaction> results;
        results.reserve(txRows.size());

        for (const auto& txRow : txRows)
        {
            const auto joins =
                orm::Joins{}
                    .add(
                        orm::join<
                            TransactionEntryRow::transactionIdField,
                            TransactionRow::idField>()
                    )
                    .add(
                        orm::join<
                            TransactionEntryRow::instrumentIdField,
                            InstrumentRow::idField>()
                    );

            const auto query = orm::Query{}.where(
                TransactionEntryRow::hasTransactionId(txRow.id.value())
            );

            const auto entryRows =
                _getCrud().getJoined<TransactionEntryRow, InstrumentRow>(
                    _getDb(),
                    joins,
                    query
                );

            auto transaction = TransactionFactory::fromRow(txRow);

            for (const auto& [entryRow, instrumentRow] : entryRows)
                transaction.addEntry(
                    TransactionFactory::fromEntryRow(entryRow, instrumentRow)
                );

            results.push_back(std::move(transaction));
        }

        return results;
    }

}   // namespace app
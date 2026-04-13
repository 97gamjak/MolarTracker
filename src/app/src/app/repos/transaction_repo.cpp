#include "transaction_repo.hpp"

#include "app/factories/transaction_factory.hpp"
#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "db/transaction.hpp"
#include "finance/transaction.hpp"
#include "orm/crud.hpp"
#include "orm/join.hpp"
#include "orm/optional.hpp"
#include "repo_errors.hpp"
#include "sql_models/instrument_row.hpp"

namespace app
{
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

            const auto idOpt = _getInstrument(instrument);

            InstrumentId instrumentId;
            if (idOpt.has_value())
                instrumentId = idOpt.value();
            else
                instrumentId = _insertInstrument(instrument);

            const auto entryRow =
                TransactionFactory::toEntryRow(entry, txId, instrumentId);

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

    /**
     * @brief get an instrument from the database
     *
     * @param row
     * @return std::optional<InstrumentId>
     */
    std::optional<InstrumentId> TransactionRepo::_getInstrument(
        const InstrumentRow& row
    )
    {
        switch (row.kind.value())
        {
            case InstrumentKind::Cash:
            {
                const auto query = orm::Query{}.where(
                    InstrumentRow::hasKind(InstrumentKind::Cash)
                );

                const auto instrument =
                    _getCrud().getUnique<InstrumentRow>(_getDb(), query);

                if (instrument.has_value())
                    return instrument->id.value();

                return std::nullopt;
            }
            case InstrumentKind::Stock:
                throw std::runtime_error(
                    "Stock instruments are not supported yet"
                );
        }

        // can not happen in theory
        throw std::runtime_error("Unknown instrument kind");
    }

    /**
     * @brief insert an instrument to the database
     *
     * @param row
     * @return InstrumentId
     */
    InstrumentId TransactionRepo::_insertInstrument(const InstrumentRow& row)
    {
        const auto result = _getCrud().insert(_getDb(), row);

        if (!result.has_value())
        {
            const auto msg = getInsertError(result.error(), "instrument");
            LOG_ERROR(msg);
            throw orm::CrudException(msg);
        }

        return InstrumentId(result.value());
    }

}   // namespace app
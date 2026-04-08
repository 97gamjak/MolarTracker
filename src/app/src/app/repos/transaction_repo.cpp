#include "transaction_repo.hpp"

#include "app/factories/transaction_factory.hpp"
#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "db/transaction.hpp"
#include "finance/transaction.hpp"
#include "orm/crud.hpp"
#include "orm/where_expr.hpp"
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
            // 1. check if instrument exists -> if not create it
            const auto instrument =
                TransactionFactory::toInstrumentRow(entry.getDetails());

            const auto idOpt = _getInstrument(instrument);

            InstrumentId instrumentId;
            if (idOpt.has_value())
                instrumentId = idOpt.value();
            else
                instrumentId = _insertInstrument(instrument);

            const auto entryRow = TransactionFactory::toTransactionEntryRow(
                entry,
                txId,
                instrumentId
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
                const auto where = orm::makeWhere(
                    row.kind,
                    InstrumentRow::tableName,
                    filter::Operator::Equal
                );

                const auto instruments =
                    orm::Crud().getAll<InstrumentRow>(_getDb(), where);

                if (instruments.size() == 1)
                    return instruments.front().id.value();

                if (instruments.empty())
                    return std::nullopt;

                throw std::runtime_error("Multiple instruments found");
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
    InstrumentId TransactionRepo::_insertInstrument(
        const InstrumentRow& row
    ) const
    {
        const auto result = orm::Crud().insert(_getDb(), row);

        if (!result.has_value())
        {
            const auto msg = getInsertError(result.error(), "instrument");
            LOG_ERROR(msg);
            throw orm::CrudException(msg);
        }

        return InstrumentId(result.value());
    }

}   // namespace app
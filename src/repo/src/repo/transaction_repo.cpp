#include "transaction_repo.hpp"

#include <algorithm>

#include "common/finance.hpp"
#include "config/id_types.hpp"
#include "db/transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/stock_data.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "logging/log_macros.hpp"
#include "orm/crud.hpp"
#include "orm/join.hpp"
#include "repo/factories/transaction_factory.hpp"
#include "sql_models/trade_leg_row.hpp"
#include "sql_models/transaction_entry_row.hpp"
#include "sql_models/transaction_option_row.hpp"
#include "sql_models/transaction_row.hpp"

REGISTER_LOG_CATEGORY("Repo.TransactionRepo");

namespace repo
{
    namespace
    {
        /**
         * @brief add the legs of a trade transaction to the database
         *
         * @param legs
         * @param txId
         * @param dbTx
         * @param crud
         * @param db
         *
         * @return CrudResult<void> An empty expected on success, or an error on
         * failure
         */
        [[nodiscard]]
        CrudResult<void> addLegs(
            const finance::TradeLegs& legs,
            TransactionId             txId,
            const db::Transaction&    dbTx,
            orm::Crud&                crud,
            db::Database&             db
        )
        {
            for (const auto& leg : legs)
            {
                const auto legRow = TransactionFactory::toLegRow(leg, txId);

                const auto legResult = crud.insert(db, dbTx, legRow);

                if (!legResult.has_value())
                {
                    return legResult.error().convert(
                        "Failed to insert trade leg for transaction ID: " +
                        txId.toString()
                    );
                }
            }
            return {};
        }

        /**
         * @brief Prepare a DomainTransaction object from a TransactionRow and
         * an optional TransactionOptionRow, this method takes a TransactionRow
         * and an optional TransactionOptionRow and constructs a
         * DomainTransaction object based on the type of transaction, allowing
         * for the creation of a complete DomainTransaction that includes all
         * relevant data for the transaction, including option details if
         * applicable.
         *
         * @param txRow The TransactionRow containing the basic transaction
         * data.
         * @param optionRow An optional TransactionOptionRow containing
         * additional details for option transactions.
         * @return finance::DomainTransaction The prepared DomainTransaction
         * object.
         */
        finance::DomainTransaction prepareDomainTx(
            const TransactionRow& txRow,
            orm::Crud&            crud,
            db::Database&         db
        )
        {
            switch (txRow.type.value())
            {
                case TransactionDataType::Option:
                {
                    const auto optionRow = crud.getUnique<TransactionOptionRow>(
                        db,
                        orm::Query{}.where(
                            TransactionOptionRow::hasTransactionId(
                                txRow.id.value()
                            )
                        )
                    );

                    if (!optionRow.has_value())
                    {
                        LOG_ERROR(
                            "Failed to retrieve option data for transaction "
                            "with "
                            "ID " +
                            txRow.id.value().toString()
                        );
                        throw orm::CrudException(
                            "Missing option data for option transaction with "
                            "ID " +
                            txRow.id.value().toString()
                        );
                    }

                    return TransactionFactory::fromOptionRow(
                        txRow,
                        optionRow.value()
                    );
                }
                case TransactionDataType::Stock:
                    return TransactionFactory::fromStockRow(txRow);
                case TransactionDataType::Cash:
                    return TransactionFactory::fromCashRow(txRow);
            }

            std::unreachable();
        }
    }   // namespace
    /**
     * @brief add a transaction to the database
     *
     * @param transaction
     * @return CrudResult<TransactionId> The ID of the newly added transaction,
     * or an error if the operation failed.
     */
    CrudResult<TransactionId> TransactionRepo::addTransaction(
        const finance::DomainTransaction& transaction
    )
    {
        db::Transaction dbTx{_getDb()};
        auto            txRow = TransactionFactory::toRow(transaction);

        const auto transactionResult = _getCrud().insert(_getDb(), txRow);

        if (!transactionResult.has_value())
        {
            return transactionResult.error().convert(
                "Failed to insert transaction with ID '" +
                txRow.id.value().toString() + "'"
            );
        }

        const auto txId = TransactionId(transactionResult.value());

        for (const auto& entry : transaction.getEntries())
        {
            // 1. check if instrument exists -> if not create it
            const auto entryRow = TransactionFactory::toEntryRow(entry, txId);

            const auto entryResult =
                _getCrud().insert(_getDb(), dbTx, entryRow);

            if (!entryResult.has_value())
            {
                return entryResult.error().convert(
                    "Failed to insert transaction entry with ID '" +
                    entryRow.id.value().toString() + "'"
                );
            }
        }

        switch (txRow.type.value())
        {
            case TransactionDataType::Stock:
            {
                const auto data =
                    std::get<finance::StockData>(transaction.getData());

                const auto readLegsResult = addLegs(
                    data.getLegs(),
                    TransactionId(transactionResult.value()),
                    dbTx,
                    _getCrud(),
                    _getDb()
                );
                if (!readLegsResult.has_value())
                {
                    return readLegsResult.error().convert(
                        "Failed to insert trade legs for transaction ID: " +
                        txId.toString()
                    );
                }
                break;
            }
            case TransactionDataType::Option:
            {
                const auto data =
                    std::get<finance::OptionData>(transaction.getData());

                const auto optionLegsResult = addLegs(
                    data.getLegs(),
                    TransactionId(transactionResult.value()),
                    dbTx,
                    _getCrud(),
                    _getDb()
                );
                if (!optionLegsResult.has_value())
                {
                    return optionLegsResult.error().convert(
                        "Failed to insert trade legs for transaction ID: " +
                        txId.toString()
                    );
                }

                const auto optionResult = _getCrud().insert(
                    _getDb(),
                    dbTx,
                    TransactionFactory::toOptionRow(data, txId)
                );

                if (!optionResult.has_value())
                {
                    return optionResult.error().convert(
                        "Failed to insert transaction option data for "
                        "transaction with ID '" +
                        txRow.id.value().toString() + "'"
                    );
                }
                break;
            }
            case TransactionDataType::Cash:
                break;
        }

        const auto commitResult = dbTx.commit();
        if (!commitResult)
        {
            const auto msg = "Failed to commit transaction: " +
                             commitResult.error().toString();
            LOG_ERROR(msg);
            throw orm::CrudException(msg);
        }

        return TransactionId(transactionResult.value());
    }

    /**
     * @brief get all transactions from the database
     *
     * @param filter The filter to apply to the transactions, this will be
     * converted to a WhereExpr and applied to the query when fetching
     * transactions from the database, if no filter is provided all transactions
     * will be returned
     *
     * @return std::vector<finance::DomainTransaction>
     */
    std::vector<finance::DomainTransaction> TransactionRepo::getTransactions(
        const finance::TransactionFilter& filter
    )
    {
        const auto query =
            orm::Query{}.where(TransactionFactory::toWhereExpr(filter));

        const auto join = orm::Joins{}.add(TransactionFactory::toJoin(filter));

        const auto txRows =
            _getCrud().getJoined<TransactionRow>(_getDb(), join, query);

        std::vector<finance::DomainTransaction> results;
        results.reserve(txRows.size());

        const auto& accountIds = filter.accountIds;

        for (const auto& [txRow] : txRows)
        {
            const auto allEntriesQuery = orm::Query{}.where(
                TransactionEntryRow::hasTransactionId(txRow.id.value())
            );
            const auto allLegsQuery = orm::Query{}.where(
                TradeLegRow::hasTransactionId(txRow.id.value())
            );

            const auto entryRows =
                _getCrud().get<TransactionEntryRow>(_getDb(), allEntriesQuery);
            const auto legRows =
                _getCrud().get<TradeLegRow>(_getDb(), allLegsQuery);

            const auto inSet = [&](const auto& row)
            { return accountIds.contains(row.accountId.value()); };

            if (!std::ranges::any_of(entryRows, inSet) &&
                !std::ranges::any_of(legRows, inSet))
            {
                LOG_WARNING(
                    "Skipping transaction with ID " +
                    txRow.id.value().toString() +
                    " because not all entries/legs match the account filter"
                );
                continue;
            }

            auto transaction = prepareDomainTx(txRow, _getCrud(), _getDb());

            for (const auto& entryRow : entryRows)
                transaction.addEntry(
                    TransactionFactory::fromEntryRow(entryRow)
                );

            for (const auto& legRow : legRows)
                transaction.addLeg(TransactionFactory::fromLegRow(legRow));

            results.push_back(std::move(transaction));
        }

        LOG_DEBUG(
            std::format(
                "Retrieved {} transactions from the database",
                results.size()
            )
        );

        return results;
    }

}   // namespace repo

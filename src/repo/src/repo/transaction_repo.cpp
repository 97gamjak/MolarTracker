#include "transaction_repo.hpp"

#include <algorithm>

#include "common/finance.hpp"
#include "config/id_types.hpp"
#include "db/transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/stock_data.hpp"
#include "logging/log_macros.hpp"
#include "orm/crud.hpp"
#include "orm/join.hpp"
#include "repo/factories/transaction_factory.hpp"
#include "repo_errors.hpp"
#include "sql_models/trade_leg_row.hpp"
#include "sql_models/transaction_entry_row.hpp"
#include "sql_models/transaction_option_row.hpp"
#include "sql_models/transaction_row.hpp"

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
         */
        void addLegs(
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
                    const auto msg =
                        getInsertError(legResult.error(), "trade leg");

                    LOG_ERROR(msg);
                    throw orm::CrudException(msg);
                }
            }
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
     * @return TransactionId
     */
    TransactionId TransactionRepo::addTransaction(
        const finance::DomainTransaction& transaction
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
            const auto entryRow = TransactionFactory::toEntryRow(entry, txId);

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

        switch (txRow.type.value())
        {
            case TransactionDataType::Stock:
            {
                const auto data =
                    std::get<finance::StockData>(transaction.getData());

                addLegs(
                    data.getLegs(),
                    TransactionId(transactionResult.value()),
                    dbTx,
                    _getCrud(),
                    _getDb()
                );
                break;
            }
            case TransactionDataType::Option:
            {
                const auto data =
                    std::get<finance::OptionData>(transaction.getData());

                addLegs(
                    data.getLegs(),
                    TransactionId(transactionResult.value()),
                    dbTx,
                    _getCrud(),
                    _getDb()
                );

                const auto optionResult = _getCrud().insert(
                    _getDb(),
                    dbTx,
                    TransactionFactory::toOptionRow(data, txId)
                );

                if (!optionResult.has_value())
                {
                    const auto msg = getInsertError(
                        optionResult.error(),
                        "transaction option"
                    );

                    LOG_ERROR(msg);
                    throw orm::CrudException(msg);
                }
                break;
            }
            case TransactionDataType::Cash:
                break;
        }

        dbTx.commit();

        return TransactionId(transactionResult.value());
    }

    /**
     * @brief get all transactions from the database
     *
     * @param accountIds The IDs of the accounts to retrieve transactions
     * for.
     * @param filter The filter to apply to the transactions, this will be
     * converted to a WhereExpr and applied to the query when fetching
     * transactions from the database, if no filter is provided all transactions
     * will be returned
     *
     * @return std::vector<finance::DomainTransaction>
     */
    std::vector<finance::DomainTransaction> TransactionRepo::getTransactions(
        const IdSet<AccountId>&           accountIds,
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

            if (!std::ranges::all_of(entryRows, inSet) ||
                !std::ranges::all_of(legRows, inSet))
            {
                if (std::ranges::any_of(entryRows, inSet) ||
                    std::ranges::any_of(legRows, inSet))
                {
                    LOG_WARNING(
                        "Skipping transaction with ID " +
                        txRow.id.value().toString() +
                        " because not all entries/legs match the account filter"
                    );
                }
                else
                {
                    LOG_TRACE(
                        "Skipping transaction with ID " +
                        txRow.id.value().toString() +
                        " because no entries/legs match the account filter"
                    );
                }
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

        return results;
    }

}   // namespace repo
#include "controller/mapper/transaction/transaction_overview_mapper.hpp"

#include "cache/account_cache.hpp"
#include "cache/stock_cache.hpp"
#include "controller/mapper/account_mapper.hpp"
#include "drafts/transaction/transaction_overview_draft.hpp"
#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "finance/transaction/transactions.hpp"

using cache::AccountCache;
using cache::AccountCacheUtils;
using cache::StockCache;
using cache::StockCacheUtils;
using drafts::CashTransactionOverview;
using drafts::StockTransactionOverview;

namespace controller
{
    namespace
    {

        /**
         * @brief Helper function to create a FinanceError for transaction
         * overview conversion failures, this function takes a transaction and
         * an existing FinanceError, and returns a new FinanceError with
         * additional context about the failure, including the transaction ID.
         *
         * @tparam TransactionType The type of the transaction (e.g.,
         * StockTransaction or CashTransaction)
         * @param transactions The transaction that failed to convert
         * @param result The existing FinanceError that caused the failure
         * @return FinanceError A new FinanceError with additional context about
         * the failure
         */
        template <typename TransactionType>
        FinanceError toOverviewError(
            const TransactionType& transactions,
            const FinanceError&    result
        )
        {
            return FinanceError::fromError(
                result,
                FinanceErrorType::InvalidTransaction,
                std::format(
                    "Failed to convert transaction with ID {} to "
                    "overview",
                    transactions.getId().toString()
                )
            );
        }

        /**
         * @brief Converts a StockTransaction to a
         * drafts::StockTransactionOverview, this will extract the relevant
         * information from the stock transaction and format it for display in
         * the transaction overview.
         *
         * @param transaction
         * @param stockCache
         * @param accountCache
         *
         * @return drafts::StockTransactionOverview
         */
        Result<StockTransactionOverview, FinanceError> _toStockOverview(
            const finance::StockTransaction&     transaction,
            const std::shared_ptr<StockCache>&   stockCache,
            const std::shared_ptr<AccountCache>& accountCache
        )
        {
            const auto instrumentId = transaction.getBaseInstrumentId();

            const auto& stock =
                StockCacheUtils::getStock(instrumentId, stockCache);

            if (!stock)
                return toOverviewError(transaction, stock.error());

            const auto& ticker = (*stock)->getTicker();

            const auto& securityAccount = AccountCacheUtils::getSecurityAccount(
                transaction.getSecurityAccountId(),
                accountCache
            );

            if (!securityAccount)
                return toOverviewError(transaction, securityAccount.error());

            const auto& cashAccount = AccountCacheUtils::getCashAccount(
                transaction.getCashAccountId(),
                accountCache
            );

            if (!cashAccount)
                return toOverviewError(transaction, cashAccount.error());

            return StockTransactionOverview(
                transaction.getTimestamp(),
                transaction.getComment(),
                transaction.getQuantity(),
                transaction.getUnitPrice(),
                transaction.getFees(),
                ticker,
                AccountMapper::toDraft(**securityAccount),
                AccountMapper::toDraft(**cashAccount)
            );
        }

        /**
         * @brief Converts a CashTransaction to a
         * drafts::CashTransactionOverview, this will extract the relevant
         * information from the cash transaction and format it for display in
         * the transaction overview.
         *
         * @param transaction
         * @param accountCache
         *
         * @return Result<drafts::CashTransactionOverview, FinanceError>
         */
        Result<drafts::CashTransactionOverview, FinanceError> _toCashOverview(
            const finance::CashTransaction&      transaction,
            const std::shared_ptr<AccountCache>& accountCache
        )
        {
            const auto& cashAccount = AccountCacheUtils::getCashAccount(
                transaction.getCashAccountId(),
                accountCache
            );

            if (!cashAccount)
                return toOverviewError(transaction, cashAccount.error());

            return drafts::CashTransactionOverview(
                transaction.getTimestamp(),
                transaction.getComment(),
                transaction.getAmount(),
                transaction.getFees(),
                AccountMapper::toDraft(**cashAccount)
            );
        }
    }   // namespace

    /**
     * @brief Converts a vector of StockTransaction to a vector of
     * drafts::StockTransactionOverview, this will iterate over the list of
     * stock transactions and convert each one to a StockTransactionOverview
     * draft using the toStockOverview function, and return the resulting list
     * of drafts for display in the transaction overview.
     *
     * @param transactions
     * @param stockCache
     * @param accountCache
     *
     * @return Result<std::vector<StockTransactionOverview>, FinanceError>
     */
    Result<std::vector<StockTransactionOverview>, FinanceError> TransactionOverviewMapper::
        toStockOverview(
            const finance::Transactions&         transactions,
            const std::shared_ptr<StockCache>&   stockCache,
            const std::shared_ptr<AccountCache>& accountCache
        )
    {
        std::vector<StockTransactionOverview> result;

        for (const auto& transaction : transactions.stocks())
        {
            const auto& overview =
                _toStockOverview(transaction, stockCache, accountCache);

            if (!overview)
                return overview.error();

            result.push_back(*overview);
        }

        return result;
    }

    /**
     * @brief Converts a vector of CashTransaction to a vector of
     * drafts::CashTransactionOverview, this will iterate over the list of cash
     * transactions and convert each one to a CashTransactionOverview draft
     * using the toCashOverview function, and return the resulting list of
     * drafts for display in the transaction overview.
     *
     * @param transactions
     * @param accountCache
     *
     * @return Result<std::vector<CashTransactionOverview>, FinanceError>
     */
    Result<std::vector<CashTransactionOverview>, FinanceError> TransactionOverviewMapper::
        toCash(
            const finance::Transactions&         transactions,
            const std::shared_ptr<AccountCache>& accountCache
        )
    {
        std::vector<drafts::CashTransactionOverview> result;

        for (const auto& transaction : transactions.cash())
        {
            const auto& overview = _toCashOverview(transaction, accountCache);

            if (!overview)
                return overview.error();

            result.push_back(*overview);
        }

        return result;
    }

}   // namespace controller

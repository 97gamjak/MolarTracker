#include "controller/mapper/transaction/transaction_overview_mapper.hpp"

#include "cache/stock_cache.hpp"
#include "drafts/transaction/transaction_overview_draft.hpp"
#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "finance/transaction/transactions.hpp"

namespace controller
{
    using drafts::StockTransactionOverview;

    namespace
    {
        StockTransactionOverview toStockOverview(
            const finance::StockTransaction&          transaction,
            const std::shared_ptr<cache::StockCache>& stockCache
        )
        {
            const auto instrumentId = transaction.getBaseInstrumentId();

            const auto& stock = stockCache->getStock(instrumentId);

            std::string ticker = "Unknown";
            if (stock)
                ticker = stock->getTicker();

            return StockTransactionOverview(
                transaction.getTimestamp(),
                transaction.getComment(),
                transaction.getQuantity(),
                transaction.getUnitPrice(),
                transaction.getFees(),
                ticker,
                transaction.getSecurityAccountId(),
                transaction.getCashAccountId()
            );
        }

        /**
         * @brief Converts a CashTransaction to a
         * drafts::CashTransactionOverview, this will extract the relevant
         * information from the cash transaction and format it for display in
         * the transaction overview.
         *
         * @param transaction
         * @return drafts::CashTransactionOverview
         */
        drafts::CashTransactionOverview toCashOverview(
            const finance::CashTransaction& transaction
        )
        {
            return drafts::CashTransactionOverview(
                transaction.getTimestamp(),
                transaction.getComment(),
                transaction.getAmount(),
                transaction.getFees(),
                transaction.getCashAccountId(),
                transaction.getExternalAccountId()
            );
        }
    }   // namespace

    std::vector<StockTransactionOverview> TransactionOverviewMapper::toStock(
        const finance::Transactions&              transactions,
        const std::shared_ptr<cache::StockCache>& stockCache
    )
    {
        std::vector<StockTransactionOverview> result;

        for (const auto& transaction : transactions.stocks())
        {
            result.push_back(toStockOverview(transaction, stockCache));
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
     * @return std::vector<drafts::CashTransactionOverview>
     */
    std::vector<drafts::CashTransactionOverview> TransactionOverviewMapper::
        toCash(const finance::Transactions& transactions)
    {
        std::vector<drafts::CashTransactionOverview> result;

        for (const auto& transaction : transactions.cash())
        {
            result.push_back(toCashOverview(transaction));
        }

        return result;
    }

}   // namespace controller

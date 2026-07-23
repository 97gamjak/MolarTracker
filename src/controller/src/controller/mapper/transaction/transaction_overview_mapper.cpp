#include "controller/mapper/transaction/transaction_overview_mapper.hpp"

#include "common/container/id_map.hpp"
#include "drafts/transaction/transaction_overview_draft.hpp"
#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "finance/transaction/transactions.hpp"

namespace controller
{
    using drafts::StockTransactionOverview;

    namespace
    {
        /**
         * @brief Converts a finance::StockTransaction to a
         * StockTransactionOverview draft, this will extract the relevant
         * information from the stock transaction and format it for display in
         * the transaction overview, including resolving the instrument ID to a
         * ticker symbol using the provided mapping of instrument IDs to names.
         *
         * @param transaction
         * @param instrumentNames
         * @return StockTransactionOverview
         */
        StockTransactionOverview toStockOverview(
            const finance::StockTransaction&        transaction,
            const IdMap<InstrumentId, std::string>& instrumentNames
        )
        {
            const auto  instrumentId = transaction.getBaseInstrumentId();
            std::string ticker       = "UNKNOWN";

            if (instrumentNames.contains(instrumentId))
                ticker = instrumentNames.at(instrumentId);

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

    /**
     * @brief Converts a vector of finance::StockTransaction to a vector of
     * StockTransactionOverview drafts, this will iterate over the list of stock
     * transactions and convert each one to a StockTransactionOverview draft
     * using the toStockOverview function, and return the resulting list of
     * drafts for display in the transaction overview.
     *
     * @param transactions
     * @param instrumentNames
     * @return std::vector<StockTransactionOverview>
     */
    std::vector<StockTransactionOverview> TransactionOverviewMapper::toStock(
        const finance::Transactions&            transactions,
        const IdMap<InstrumentId, std::string>& instrumentNames
    )
    {
        std::vector<StockTransactionOverview> result;

        for (const auto& transaction : transactions.stocks())
        {
            result.push_back(toStockOverview(transaction, instrumentNames));
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

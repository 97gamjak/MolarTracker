#include "transaction_overview_mapper.hpp"

#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "finance/transaction/transactions.hpp"

namespace controller
{

    namespace
    {
        drafts::StockTransactionOverview toStockOverview(
            const finance::StockTransaction&                 transaction,
            const unorderedIdMap<InstrumentId, std::string>& instrumentNames
        )
        {
            const auto  instrumentId = transaction.getBaseInstrumentId();
            std::string ticker       = "UNKNOWN";

            if (instrumentNames.contains(instrumentId))
                ticker = instrumentNames.at(instrumentId);

            return drafts::StockTransactionOverview(
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

    std::vector<drafts::StockTransactionOverview> TransactionOverviewMapper::
        toStock(
            const finance::Transactions&      transactions,
            const instrumentMap<std::string>& instrumentNames
        )
    {
        std::vector<drafts::StockTransactionOverview> result;

        for (const auto& transaction : transactions.stocks())
        {
            result.push_back(toStockOverview(transaction, instrumentNames));
        }

        return result;
    }

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

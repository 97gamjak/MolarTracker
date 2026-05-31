#include "finance/transaction/transaction_converter.hpp"

#include <expected>
#include <vector>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/account.hpp"
#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "finance/transaction/transaction_entry.hpp"

namespace finance
{
    DomainTransaction TransactionConverter::toDomain(
        const CashTransaction& transaction
    )
    {
        return DomainTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            CashData{},
            transaction.getTransactionEntries()
        };
    }

    DomainTransaction TransactionConverter::toDomain(
        const StockTransaction& transaction
    )
    {
        return DomainTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            transaction.getTradeData(),
            transaction.getTransactionEntries()
        };
    }

    std::expected<CashTransaction, TransactionConversionError> TransactionConverter::
        toCash(
            const DomainTransaction& transaction,
            const std::vector<Account>& /*accounts*/
        )
    {
        const auto& entries = transaction.getEntries();
        if (entries.empty())
        {
            return std::unexpected(
                TransactionConversionError{"No cash entries found"}
            );
        }

        auto amountEntries = entries.filter(TransactionEntryType::General);
        auto feeEntries    = entries.filter(TransactionEntryType::Fees);

        const TransactionEntries nonExternalAmountEntries;
        const TransactionEntries nonExternalAmountEntries;

        return CashTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            CashData{},
            transaction.getEntries()
        };
    }

    StockTransaction TransactionConverter::toStock(
        const DomainTransaction& transaction
    )
    {
        return StockTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            transaction.getTradeData(),
            transaction.getTransactionEntries()
        };
    }

}   // namespace finance

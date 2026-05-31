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
        toCash(const DomainTransaction& transaction, const Accounts& accounts)
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

        if (amountEntries.size() != 2)
        {
            return std::unexpected(
                TransactionConversionError{"Invalid number of amount entries"}
            );
        }

        if (feeEntries.size() != 2 || !feeEntries.empty())
        {
            return std::unexpected(
                TransactionConversionError{"Invalid number of fee entries"}
            );
        }

        const auto filteredAccounts = accounts.filterExternal(false);
        const auto externalAccounts = accounts.filterExternal(true);

        AccountId internalAccountId;
        AccountId externalAccountId;

        if (filteredAccounts.contains(amountEntries[0].getAccountId()))
        {
            internalAccountId = amountEntries[0].getAccountId();
            externalAccountId = amountEntries[1].getAccountId();
        }
        else if (filteredAccounts.contains(amountEntries[1].getAccountId()))
        {
            internalAccountId = amountEntries[1].getAccountId();
            externalAccountId = amountEntries[0].getAccountId();
        }
        else
        {
            return std::unexpected(
                TransactionConversionError{"No internal account found"}
            );
        }

        if (!externalAccounts.contains(externalAccountId))
        {
            return std::unexpected(
                TransactionConversionError{"No external account found"}
            );
        }

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

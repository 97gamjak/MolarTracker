#include "finance/transaction/transaction_converter.hpp"

#include <expected>
#include <variant>

#include "config/id_types.hpp"
#include "error/finance_error.hpp"
#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/stock_data.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "utils/finance.hpp"

namespace finance
{
    namespace
    {
        [[nodiscard]]
        FinanceError toDomainTransactionError(
            const FinanceError& error,
            const std::string&  message
        )
        {
            return error.convert(
                FinanceErrorType::InvalidTransaction,
                "Could not convert" + message + "to DomainTransaction"
            );
        }
    }   // namespace
    /**
     * @brief Converts a CashTransaction to a DomainTransaction, this
     * will take the relevant information from the cash transaction and format
     * it into a DomainTransaction, including creating the appropriate
     * transaction entries for the cash flows associated with the transaction.
     *
     * @param transaction
     * @param accounts
     *
     * @return FinanceResult<DomainTransaction>
     */
    FinanceResult<DomainTransaction> TransactionConverter::toDomain(
        const CashTransaction& transaction,
        const AccountsView&    accounts
    )
    {
        const auto cashId = transaction.getCashAccountId();
        const auto externalAccountId =
            accounts.getCorrespondingExternalAccountId(cashId);

        if (!externalAccountId)
        {
            return toDomainTransactionError(
                externalAccountId.error(),
                "CashTransaction"
            );
        }

        return DomainTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            CashData{},
            transaction.getEntries(externalAccountId.value())
        };
    }

    /**
     * @brief Converts a finance::StockTransaction to a DomainTransaction, this
     * will take the relevant information from the stock transaction and format
     * it into a DomainTransaction, including creating the appropriate
     * transaction entries for the stock trades associated with the transaction.
     *
     * @param transaction
     * @param accounts
     *
     * @return FinanceResult<DomainTransaction>
     */
    FinanceResult<DomainTransaction> TransactionConverter::toDomain(
        const StockTransaction& transaction,
        const AccountsView&     accounts
    )
    {
        const auto externalAccountId =
            accounts.getCorrespondingExternalAccountId(
                transaction.getCashAccountId()
            );

        if (!externalAccountId)
        {
            return toDomainTransactionError(
                externalAccountId.error(),
                "StockTransaction"
            );
        }

        return DomainTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            transaction.getStockData(),
            transaction.getEntries(*externalAccountId)
        };
    }

    /**
     * @brief Converts an OptionTransaction to a DomainTransaction, this
     * will take the relevant information from the option transaction and format
     * it into a DomainTransaction, including creating the appropriate
     * transaction entries for the option trades associated with the
     * transaction.
     *
     * @param transaction
     * @param accounts
     *
     * @return FinanceResult<DomainTransaction>
     */
    FinanceResult<DomainTransaction> TransactionConverter::toDomain(
        const OptionTransaction& transaction,
        const AccountsView&      accounts
    )
    {
        const auto externalAccountId =
            accounts.getCorrespondingExternalAccountId(
                transaction.getCashAccountId()
            );

        if (!externalAccountId)
        {
            return toDomainTransactionError(
                externalAccountId.error(),
                "OptionTransaction"
            );
        }

        return DomainTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            transaction.getOptionData(),
            transaction.getEntries(*externalAccountId)
        };
    }

    /**
     * @brief Converts a DomainTransaction to a CashTransaction, this
     * will take the relevant information from the DomainTransaction and format
     * it into a CashTransaction, including creating the appropriate transaction
     * entries for the cash flows associated with the transaction.
     *
     * @param transaction
     * @param accounts
     * @return FinanceResult<CashTransaction>
     */
    FinanceResult<CashTransaction> TransactionConverter::toCash(
        const DomainTransaction& transaction,
        const AccountsView&      accounts
    )
    {
        const auto& entries = transaction.getEntries();
        if (entries.empty())
        {
            return FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid DomainTransaction: No cash entries found"
            };
        }

        auto amountEntries = entries.filter(TransactionEntryType::General);
        auto feeEntries    = entries.filter(TransactionEntryType::Fees);

        if (amountEntries.size() != 2)
        {
            return FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid DomainTransaction: Invalid number of amount entries"
            };
        }

        if (feeEntries.size() != 2 && !feeEntries.empty())
        {
            return FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid DomainTransaction: Invalid number of fee entries"
            };
        }

        size_t internalIndex{};

        if (!accounts.isExternal(amountEntries[0].getAccountId()))
            internalIndex = 0;
        else if (!accounts.isExternal(amountEntries[1].getAccountId()))
            internalIndex = 1;
        else
        {
            return FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid DomainTransaction: No internal account found"
            };
        }

        const auto internalAccountId =
            amountEntries[internalIndex].getAccountId();
        const auto externalAccountId =
            amountEntries[1 - internalIndex].getAccountId();
        const auto amount = amountEntries[internalIndex].getCash();

        if (!accounts.isExternal(externalAccountId))
        {
            return FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid DomainTransaction: No external account found"
            };
        }

        Cash fees{amountEntries[0].getCurrency(), 0};

        if (feeEntries.size() == 2)
        {
            if (internalAccountId != feeEntries[0].getAccountId() &&
                internalAccountId != feeEntries[1].getAccountId())
            {
                return FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    "Invalid DomainTransaction: Invalid fee entry accounts"
                };
            }

            if (externalAccountId != feeEntries[0].getAccountId() &&
                externalAccountId != feeEntries[1].getAccountId())
            {
                return FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    "Invalid DomainTransaction: Invalid fee entry accounts"
                };
            }

            if (internalAccountId == feeEntries[0].getAccountId())
                fees += feeEntries[0].getCash();
            else
                fees += feeEntries[1].getCash();
        }

        return CashTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            internalAccountId,
            externalAccountId,
            amount,
            fees,
            transaction.getComment()
        };
    }

    /**
     * @brief Converts a DomainTransaction to a finance::StockTransaction, this
     * will take the relevant information from the DomainTransaction and format
     * it into a StockTransaction, including creating the appropriate
     * transaction entries for the stock trades associated with the transaction.
     *
     * @param transaction
     * @return FinanceResult<StockTransaction>
     */
    FinanceResult<StockTransaction> TransactionConverter::toStock(
        const DomainTransaction& transaction
    )
    {
        const auto& entries = transaction.getEntries();
        if (entries.empty())
        {
            return FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid DomainTransaction: No cash entries found"
            };
        }

        auto amountEntries = entries.filter(TransactionEntryType::General);
        auto feeEntries    = entries.filter(TransactionEntryType::Fees);

        if (amountEntries.size() != 1)
        {
            return FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid DomainTransaction: Invalid number of amount entries"
            };
        }

        const auto cashAccountId = amountEntries[0].getAccountId();

        if (feeEntries.size() != 2 && !feeEntries.empty())
        {
            return FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid DomainTransaction: Invalid number of fee entries"
            };
        }

        Cash fees{amountEntries[0].getCurrency(), 0};
        auto externalAccountId = AccountId::invalid();

        if (feeEntries.size() == 2)
        {
            if (cashAccountId != feeEntries[0].getAccountId() &&
                cashAccountId != feeEntries[1].getAccountId())
            {
                return FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    "Invalid DomainTransaction: Invalid fee entry accounts"
                };
            }

            if (cashAccountId != feeEntries[0].getAccountId() &&
                cashAccountId != feeEntries[1].getAccountId())
            {
                return FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    "Invalid DomainTransaction: Invalid fee entry accounts"
                };
            }

            if (cashAccountId == feeEntries[0].getAccountId())
            {
                externalAccountId  = feeEntries[1].getAccountId();
                fees              += feeEntries[0].getCash();
            }
            else
            {
                externalAccountId  = feeEntries[0].getAccountId();
                fees              += feeEntries[1].getCash();
            }
        }

        if (!std::holds_alternative<StockData>(transaction.getData()))
        {
            return FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid DomainTransaction: Invalid transaction data"
            };
        }

        const auto data = std::get<StockData>(transaction.getData());

        const auto& legs = data.getLegs();
        if (legs.size() != 1)
        {
            return FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid DomainTransaction: Invalid trade legs"
            };
        }

        return StockTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            legs[0].getInstrumentId(),
            legs[0].getAccountId(),
            amountEntries[0].getAccountId(),
            externalAccountId,
            legs[0].getQuantity(),
            legs[0].getUnitPrice(),
            fees,
            std::get<StockData>(transaction.getData()).getPositionId(),
            transaction.getComment()
        };
    }

}   // namespace finance

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
#include "logging/log_macros.hpp"
#include "utils/finance.hpp"

REGISTER_LOG_CATEGORY("Finance.Transaction.TransactionConverter")

namespace finance
{
    /**
     * @brief Converts a CashTransaction to a DomainTransaction, this
     * will take the relevant information from the cash transaction and format
     * it into a DomainTransaction, including creating the appropriate
     * transaction entries for the cash flows associated with the transaction.
     *
     * @param transaction
     * @param accounts
     *
     * @return DomainTransaction
     */
    DomainTransaction TransactionConverter::toDomain(
        const CashTransaction& transaction,
        const Accounts&        accounts
    )
    {
        const auto externalAccountId =
            accounts.getCorrespondingExternalAccountId(
                transaction.getCashAccountId()
            );

        if (!externalAccountId.isValid())
        {
            throw std::runtime_error(
                "No corresponding external account found for cash account: " +
                transaction.getCashAccountId().toString()
            );
        }

        return DomainTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            CashData{},
            transaction.getEntries(externalAccountId)
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
     * @return DomainTransaction
     */
    DomainTransaction TransactionConverter::toDomain(
        const StockTransaction& transaction,
        const Accounts&         accounts
    )
    {
        const auto externalAccountId =
            accounts.getCorrespondingExternalAccountId(
                transaction.getCashAccountId()
            );

        if (!externalAccountId.isValid())
        {
            throw std::runtime_error(
                "No corresponding external account found for cash account: " +
                transaction.getCashAccountId().toString()
            );
        }
        return DomainTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            transaction.getStockData(),
            transaction.getEntries(externalAccountId)
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
     * @return DomainTransaction
     */
    DomainTransaction TransactionConverter::toDomain(
        const OptionTransaction& transaction,
        const Accounts&          accounts
    )
    {
        const auto externalAccountId =
            accounts.getCorrespondingExternalAccountId(
                transaction.getCashAccountId()
            );

        if (!externalAccountId.isValid())
        {
            throw std::runtime_error(
                "No corresponding external account found for cash account: " +
                transaction.getCashAccountId().toString()
            );
        }

        return DomainTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            transaction.getOptionData(),
            transaction.getEntries(externalAccountId)
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
        const Accounts&          accounts
    )
    {
        const auto& entries = transaction.getEntries();
        if (entries.empty())
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                std::format(
                    "No cash entries found for transaction with Id {}",
                    transaction.getId().toString()
                )
            };

            LOG_ERROR(error.toString());

            return error;
        }

        auto amountEntries = entries.filter(TransactionEntryType::General);
        auto feeEntries    = entries.filter(TransactionEntryType::Fees);

        if (amountEntries.size() != 2)
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid number of amount entries for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
        }

        if (feeEntries.size() != 2 && !feeEntries.empty())
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid number of fee entries for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
        }

        const auto internalAccounts = accounts.filterExternal(false);
        const auto externalAccounts = accounts.filterExternal(true);

        size_t internalIndex{};

        if (internalAccounts.contains(amountEntries[0].getAccountId()))
            internalIndex = 0;
        else if (internalAccounts.contains(amountEntries[1].getAccountId()))
            internalIndex = 1;
        else
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "No internal account found for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
        }

        const auto internalAccountId =
            amountEntries[internalIndex].getAccountId();
        const auto externalAccountId =
            amountEntries[1 - internalIndex].getAccountId();
        const auto amount = amountEntries[internalIndex].getCash();

        if (!externalAccounts.contains(externalAccountId))
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "No external account found for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
        }

        Cash fees{amountEntries[0].getCurrency(), 0};

        if (feeEntries.size() == 2)
        {
            if (internalAccountId != feeEntries[0].getAccountId() &&
                internalAccountId != feeEntries[1].getAccountId())
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    "Invalid fee entry accounts for transaction with Id " +
                        transaction.getId().toString()
                };
                LOG_ERROR(error.toString());
                return error;
            }

            if (externalAccountId != feeEntries[0].getAccountId() &&
                externalAccountId != feeEntries[1].getAccountId())
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    "Invalid fee entry accounts for transaction with Id " +
                        transaction.getId().toString()
                };
                LOG_ERROR(error.toString());
                return error;
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
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "No cash entries found for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
        }

        auto amountEntries = entries.filter(TransactionEntryType::General);
        auto feeEntries    = entries.filter(TransactionEntryType::Fees);

        if (amountEntries.size() != 1)
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid number of amount entries for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
        }

        const auto cashAccountId = amountEntries[0].getAccountId();

        if (feeEntries.size() != 2 && !feeEntries.empty())
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid number of fee entries for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
        }

        Cash fees{amountEntries[0].getCurrency(), 0};
        auto externalAccountId = AccountId::invalid();

        if (feeEntries.size() == 2)
        {
            if (cashAccountId != feeEntries[0].getAccountId() &&
                cashAccountId != feeEntries[1].getAccountId())
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    "Invalid fee entry accounts for transaction with Id " +
                        transaction.getId().toString()
                };
                LOG_ERROR(error.toString());
                return error;
            }

            if (cashAccountId != feeEntries[0].getAccountId() &&
                cashAccountId != feeEntries[1].getAccountId())
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    "Invalid fee entry accounts for transaction with Id " +
                        transaction.getId().toString()
                };
                LOG_ERROR(error.toString());
                return error;
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
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid transaction data for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
        }

        const auto data = std::get<StockData>(transaction.getData());

        const auto& legs = data.getLegs();
        if (legs.size() != 1)
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid trade legs for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
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
            legs[0].getAmount(),
            fees,
            std::get<StockData>(transaction.getData()).getPositionId(),
            transaction.getComment()
        };
    }

    FinanceResult<OptionTransaction> TransactionConverter::toOption(
        const DomainTransaction& transaction
    )
    {
        const auto& entries = transaction.getEntries();
        if (entries.empty())
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "No cash entries found for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
        }

        auto amountEntries = entries.filter(TransactionEntryType::General);
        auto feeEntries    = entries.filter(TransactionEntryType::Fees);

        if (amountEntries.size() != 2)
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid number of amount entries for transaction with Id " +
                    transaction.getId().toString() + " Expected 1, got " +
                    std::to_string(amountEntries.size())
            };
            LOG_ERROR(error.toString());
            return error;
        }

        const auto cashAccountId = amountEntries[0].getAccountId();

        if (feeEntries.size() != 2 && !feeEntries.empty())
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid number of fee entries for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
        }

        Cash fees{amountEntries[0].getCurrency(), 0};
        auto externalAccountId = AccountId::invalid();

        if (feeEntries.size() == 2)
        {
            if (cashAccountId != feeEntries[0].getAccountId() &&
                cashAccountId != feeEntries[1].getAccountId())
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    "Invalid fee entry accounts for transaction with Id " +
                        transaction.getId().toString()
                };
                LOG_ERROR(error.toString());
                return error;
            }

            if (cashAccountId != feeEntries[0].getAccountId() &&
                cashAccountId != feeEntries[1].getAccountId())
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    "Invalid fee entry accounts for transaction with Id " +
                        transaction.getId().toString()
                };
                LOG_ERROR(error.toString());
                return error;
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

        if (!std::holds_alternative<OptionData>(transaction.getData()))
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid transaction data for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
        }

        const auto data = std::get<OptionData>(transaction.getData());

        const auto& legs = data.getLegs();
        if (legs.size() != 1)
        {
            const auto error = FinanceError{
                FinanceErrorType::InvalidTransaction,
                "Invalid trade legs for transaction with Id " +
                    transaction.getId().toString()
            };
            LOG_ERROR(error.toString());
            return error;
        }

        return OptionTransaction{
            transaction.getId(),
            transaction.getTimestamp(),
            transaction.getStatus(),
            legs[0].getInstrumentId(),
            legs[0].getAccountId(),
            amountEntries[0].getAccountId(),
            externalAccountId,
            legs[0].getQuantity(),
            legs[0].getAmount(),
            fees,
            legs[0].getPositionId(),
            data.getAction(),
            data.getBuySell(),
            data.getRolledOption(),
            transaction.getComment()
        };
    }

}   // namespace finance

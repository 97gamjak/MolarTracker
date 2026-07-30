#include "finance/transaction/transaction_converter.hpp"

#include <cassert>
#include <variant>

#include "common/finance.hpp"
#include "config/id_types.hpp"
#include "error/finance_error.hpp"
#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/stock_data.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("Finance.Transaction.TransactionConverter")

namespace finance
{
    namespace
    {
        FinanceResult<void> _checkEntries(
            TransactionId                      transactionId,
            TransactionDataType                transactionType,
            const finance::TransactionEntries& amountEntries,
            const finance::TransactionEntries& feeEntries
        )
        {
            if (amountEntries.empty() && feeEntries.empty())
            {
                return FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    std::format(
                        "No transaction entries found for {} transaction with "
                        "Id {}",
                        TransactionDataTypeMeta::toString(transactionType),
                        transactionId.toString()
                    )
                };
            }

            size_t validNumberOfAmountEntries = 0;

            switch (transactionType)
            {
                case TransactionDataType::Cash:
                case TransactionDataType::Option:
                    validNumberOfAmountEntries = 2;
                    break;
                case TransactionDataType::Stock:
                    validNumberOfAmountEntries = 1;
                    break;
            }

            if (amountEntries.size() != validNumberOfAmountEntries)
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    std::format(
                        "Invalid number of amount entries for {} transaction "
                        "with Id {}",
                        TransactionDataTypeMeta::toString(transactionType),
                        transactionId.toString()
                    )
                };
                LOG_ERROR(error.toString());
                return error;
            }

            if (feeEntries.size() != 2 && !feeEntries.empty())
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    std::format(
                        "Invalid number of fee entries for {} transaction "
                        "with Id {}",
                        TransactionDataTypeMeta::toString(transactionType),
                        transactionId.toString()
                    )
                };
                LOG_ERROR(error.toString());
                return error;
            }

            return FinanceResult<void>{};
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

        const auto amountEntries =
            entries.filter(TransactionEntryType::General);
        const auto feeEntries = entries.filter(TransactionEntryType::Fees);

        const auto checkEntires = _checkEntries(
            transaction.getId(),
            transaction.getType(),
            amountEntries,
            feeEntries
        );

        if (!checkEntires)
            return checkEntires.error();

        const auto internalAccounts = accounts.filterExternal(false);
        const auto externalAccounts = accounts.filterExternal(true);

        AccountId internalAccountId = AccountId::invalid();
        AccountId externalAccountId = AccountId::invalid();

        MT_ASSERT(amountEntries.size() == 2, FinanceErrorType::AssertionFailed);

        if (internalAccounts.contains(amountEntries[0].getAccountId()))
        {
            internalAccountId = amountEntries[0].getAccountId();
            externalAccountId = amountEntries[1].getAccountId();
        }
        else if (internalAccounts.contains(amountEntries[1].getAccountId()))
        {
            internalAccountId = amountEntries[1].getAccountId();
            externalAccountId = amountEntries[0].getAccountId();
        }
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

        // sanity check that the external account is indeed external
        MT_ASSERT(
            externalAccounts.contains(externalAccountId),
            FinanceErrorType::AssertionFailed
        );

        const auto amount = amountEntries[0].getCash();
        Cash       fees{amountEntries[0].getCurrency(), 0};

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

        auto amountEntries = entries.filter(TransactionEntryType::General);
        auto feeEntries    = entries.filter(TransactionEntryType::Fees);

        const auto checkEntires = _checkEntries(
            transaction.getId(),
            transaction.getType(),
            amountEntries,
            feeEntries
        );

        if (!checkEntires)
            return checkEntires.error();

        const auto cashAccountId = amountEntries[0].getAccountId();

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

    /**
     * @brief Converts a DomainTransaction to a finance::OptionTransaction, this
     * will take the relevant information from the DomainTransaction and format
     * it into an OptionTransaction, including creating the appropriate
     * transaction entries for the option trades associated with the
     * transaction.
     *
     * @param transaction
     * @return FinanceResult<OptionTransaction>
     */
    FinanceResult<OptionTransaction> TransactionConverter::toOption(
        const DomainTransaction& transaction
    )
    {
        const auto& entries = transaction.getEntries();

        auto amountEntries = entries.filter(TransactionEntryType::General);
        auto feeEntries    = entries.filter(TransactionEntryType::Fees);

        const auto checkEntires = _checkEntries(
            transaction.getId(),
            transaction.getType(),
            amountEntries,
            feeEntries
        );

        if (!checkEntires)
            return checkEntires.error();

        const auto cashAccountId = amountEntries[0].getAccountId();

        Cash fees{amountEntries[0].getCurrency(), 0};
        auto externalAccountId = AccountId::invalid();

        if (feeEntries.size() == 2)
        {
            if (cashAccountId != feeEntries[0].getAccountId() &&
                cashAccountId != feeEntries[1].getAccountId())
            {
                const auto error = FinanceError{
                    FinanceErrorType::InvalidTransaction,
                    "Invalid fee entry accounts for option transaction with "
                    "Id " +
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
                    "Invalid fee entry accounts for option transaction with "
                    "Id " +
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
                "Invalid transaction data for option transaction with Id " +
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

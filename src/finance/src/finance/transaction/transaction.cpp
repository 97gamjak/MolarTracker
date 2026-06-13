#include "finance/transaction/transaction.hpp"

#include <utility>

#include "config/id_types.hpp"
#include "utils/finance.hpp"

namespace finance
{

    /**
     * @brief Construct a new Transaction object
     *
     * @param id
     * @param timestamp
     * @param status
     * @param comment
     */
    BaseTransaction::BaseTransaction(
        TransactionId              id,
        Timestamp                  timestamp,
        TransactionStatus          status,
        std::optional<std::string> comment
    )
        : _id(id),
          _timestamp(timestamp),
          _status(status),
          _comment(std::move(comment))
    {
    }

    /**
     * @brief Get a string representation of the transaction, this is used for
     * logging and debugging purposes
     *
     * @return std::string
     */
    std::string BaseTransaction::toString() const
    {
        std::string result = "Transaction {\n";

        // clang-format off
        result += "  ID: " + _id.toString() + "\n";
        result += "  Timestamp: " + _timestamp.humanReadable() + "\n";
        result += "  Status: " + TransactionStatusMeta::toString(_status) + "\n";
        result += "  Data: ";
        if (_comment.has_value())
            result += "  Comment: " + _comment.value() + "\n";
        result += "}";
        // clang-format on

        return result;
    }

    /**
     * @brief Gets the ID of the transaction.
     *
     * @return TransactionId The ID of the transaction.
     */
    TransactionId BaseTransaction::getId() const { return _id; }

    /**
     * @brief Gets the timestamp of the transaction.
     *
     * @return Timestamp The timestamp of the transaction.
     */
    Timestamp BaseTransaction::getTimestamp() const { return _timestamp; }

    /**
     * @brief Gets the status of the transaction.
     *
     * @return TransactionStatus The status of the transaction.
     */
    TransactionStatus BaseTransaction::getStatus() const { return _status; }

    /**
     * @brief Gets the comment associated with the transaction.
     *
     * @return std::optional<std::string> The comment, if it exists.
     */
    std::optional<std::string> BaseTransaction::getComment() const
    {
        return _comment;
    }

    /**
     * @brief Sets the ID of the transaction.
     *
     * @param id The new ID to set.
     */
    void BaseTransaction::setId(TransactionId id) { _id = id; }

    Cash Transaction::getFees() const { return _fees; }

    AccountId Transaction::getCashAccountId() const { return _cashAccount; }

    AccountId Transaction::getExternalAccountId() const
    {
        return _externalAccount;
    }

    Transaction::Transaction(
        TransactionId              id,
        Timestamp                  timestamp,
        TransactionStatus          status,
        AccountId                  cashAccount,
        AccountId                  externalAccount,
        Cash                       fees,
        std::optional<std::string> comment
    )
        : BaseTransaction(id, timestamp, status, std::move(comment)),
          _cashAccount(cashAccount),
          _externalAccount(externalAccount),
          _fees(fees)
    {
    }

    /**
     * @brief Get the fee entry for the transaction, this will create a
     * transaction entry for the fees of the transaction, if the entry is
     * for the external account it will negate the fees to reflect the cash
     * flow correctly.
     *
     * @param external
     * @return TransactionEntry
     */
    TransactionEntry Transaction::_getFeeEntry(
        std::optional<AccountId> externalAccount
    ) const
    {
        const auto fees      = getFees();
        const auto accountId = externalAccount.has_value()
                                   ? externalAccount.value()
                                   : getCashAccountId();
        const auto amount    = externalAccount.has_value() ? -fees : fees;
        return TransactionEntry{
            TransactionEntryId::invalid(),
            accountId,
            amount,
            TransactionEntryType::Fees
        };
    }

}   // namespace finance
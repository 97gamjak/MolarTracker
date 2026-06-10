#include "finance/transaction/transaction.hpp"

#include <utility>

#include "config/finance.hpp"
#include "config/id_types.hpp"

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
    Transaction::Transaction(
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
    std::string Transaction::toString() const
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
    TransactionId Transaction::getId() const { return _id; }

    /**
     * @brief Gets the timestamp of the transaction.
     *
     * @return Timestamp The timestamp of the transaction.
     */
    Timestamp Transaction::getTimestamp() const { return _timestamp; }

    /**
     * @brief Gets the status of the transaction.
     *
     * @return TransactionStatus The status of the transaction.
     */
    TransactionStatus Transaction::getStatus() const { return _status; }

    /**
     * @brief Gets the comment associated with the transaction.
     *
     * @return std::optional<std::string> The comment, if it exists.
     */
    std::optional<std::string> Transaction::getComment() const
    {
        return _comment;
    }

    /**
     * @brief Sets the ID of the transaction.
     *
     * @param id The new ID to set.
     */
    void Transaction::setId(TransactionId id) { _id = id; }

}   // namespace finance
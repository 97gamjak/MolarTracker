#include "finance/transaction.hpp"

#include <utility>

#include "config/id_types.hpp"

namespace finance
{
    /**
     * @brief Construct a new Transaction:: Transaction object
     *
     * @param id
     * @param timestamp
     * @param status
     * @param comment
     */
    Transaction::Transaction(
        TransactionId     id,
        Timestamp         timestamp,
        TransactionStatus status,
        std::string       comment
    )
        : _id(id),
          _timestamp(timestamp),
          _status(status),
          _comment(std::move(comment))
    {
    }

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
     * @brief Construct a new Transaction:: Transaction object without a comment
     *
     * @param id
     * @param timestamp
     * @param status
     */
    Transaction::Transaction(
        TransactionId     id,
        Timestamp         timestamp,
        TransactionStatus status
    )
        : _id(id), _timestamp(timestamp), _status(status)
    {
    }

    TransactionId Transaction::getId() const { return _id; }

    Timestamp Transaction::getTimestamp() const { return _timestamp; }

    TransactionStatus Transaction::getStatus() const { return _status; }

    std::optional<std::string> Transaction::getComment() const
    {
        return _comment;
    }

    const std::vector<TransactionEntry>& Transaction::getEntries() const
    {
        return _entries;
    }

}   // namespace finance
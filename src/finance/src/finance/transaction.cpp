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

    /**
     * @brief Construct a new Transaction:: Transaction object
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
     * @brief Gets the entries associated with the transaction.
     *
     * @return const std::vector<TransactionEntry>& The entries of the
     * transaction.
     */
    const std::vector<TransactionEntry>& Transaction::getEntries() const
    {
        return _entries;
    }

    /**
     * @brief Adds an entry to the transaction.
     *
     * @param entry The TransactionEntry to add.
     */
    void Transaction::addEntry(const TransactionEntry& entry)
    {
        _entries.push_back(entry);
    }

}   // namespace finance
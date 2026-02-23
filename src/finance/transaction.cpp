#include "transaction.hpp"

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

}   // namespace finance
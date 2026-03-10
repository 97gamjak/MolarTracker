#include "transaction.hpp"

#include <utility>

namespace finance
{
    /*
    // clang-format off
    // NOLINTBEGIN
    #define TRANSACTION_STATUS_LIST(X) \
        X(Completed) \
        X(Deleted)

    MSTD_ENUM(TransactionStatus, std::uint8_t, TRANSACTION_STATUS_LIST)
    // NOLINTEND
    // clang-format on
    */

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
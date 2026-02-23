#ifndef __FINANCE__TRANSACTION_HPP__
#define __FINANCE__TRANSACTION_HPP__

#include <optional>
#include <string>

#include "config/id_types.hpp"
#include "mstd/enum.hpp"
#include "utils/timestamp.hpp"

namespace finance
{
    // clang-format off
    // NOLINTBEGIN
    #define TRANSACTION_STATUS_LIST(X) \
        X(Completed) \
        X(Deleted)

    MSTD_ENUM(TransactionStatus, std::uint8_t, TRANSACTION_STATUS_LIST)
    // NOLINTEND
    // clang-format on

    /**
     * @brief A class representing a financial transaction, which may involve
     * multiple accounts and cash movements. This class serves as a base for
     * more specific transaction types (e.g., deposits, withdrawals, transfers).
     *
     */
    class Transaction
    {
       private:
        /// The unique identifier for the transaction
        TransactionId _id;

        /// The timestamp when the transaction was created
        Timestamp _timestamp;

        /// The status of the transaction (e.g., completed, deleted)
        TransactionStatus _status;

        /// An optional comment or description for the transaction
        std::optional<std::string> _comment;

       public:
        explicit Transaction(
            TransactionId     id,
            Timestamp         timestamp,
            TransactionStatus status,
            std::string       comment
        );

        explicit Transaction(
            TransactionId     id,
            Timestamp         timestamp,
            TransactionStatus status
        );
    };

}   // namespace finance

#endif   // __FINANCE__TRANSACTION_HPP__

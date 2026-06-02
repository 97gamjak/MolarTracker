#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_HPP__

#include <optional>
#include <string>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "utils/timestamp.hpp"

namespace finance
{
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
            TransactionId              id,
            Timestamp                  timestamp,
            TransactionStatus          status,
            std::optional<std::string> comment = std::nullopt
        );
        virtual ~Transaction() = default;

        [[nodiscard]] TransactionId              getId() const;
        [[nodiscard]] Timestamp                  getTimestamp() const;
        [[nodiscard]] TransactionStatus          getStatus() const;
        [[nodiscard]] std::optional<std::string> getComment() const;

        void setId(TransactionId id);

        [[nodiscard]] std::string toString() const;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_HPP__
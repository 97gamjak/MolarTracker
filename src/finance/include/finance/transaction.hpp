#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION_HPP__

#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "transaction_entry.hpp"
#include "utils/timestamp.hpp"

namespace finance
{
    // TODO: replace these
    struct CashData
    {
    };
    struct TradeData
    {
    };

    using TransactionData = std::variant<CashData>;

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

        /// The data associated with the transaction
        TransactionData _data;

        /// A list of entries associated with the transaction, each entry
        /// represents a specific cash movement or account change related to the
        /// transaction
        std::vector<TransactionEntry> _entries;

       public:
        explicit Transaction(
            TransactionId     id,
            Timestamp         timestamp,
            TransactionStatus status,
            std::string       comment
        );

        explicit Transaction(
            TransactionId              id,
            Timestamp                  timestamp,
            TransactionStatus          status,
            std::optional<std::string> comment
        );

        explicit Transaction(
            TransactionId     id,
            Timestamp         timestamp,
            TransactionStatus status
        );

        [[nodiscard]] TransactionId                        getId() const;
        [[nodiscard]] Timestamp                            getTimestamp() const;
        [[nodiscard]] TransactionStatus                    getStatus() const;
        [[nodiscard]] std::optional<std::string>           getComment() const;
        [[nodiscard]] const std::vector<TransactionEntry>& getEntries() const;
        [[nodiscard]] std::vector<TransactionEntry>&       getEntries();

        void setId(TransactionId id);
        void addEntry(const TransactionEntry& entry);
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION_HPP__

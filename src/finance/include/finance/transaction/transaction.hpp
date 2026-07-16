#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_HPP__

#include <optional>
#include <string>

#include "config/id_types.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "utils/cash.hpp"
#include "utils/finance.hpp"
#include "utils/timestamp.hpp"

namespace finance
{
    /**
     * @brief Enum representing the status of a transaction, such as whether it
     * is completed, pending, or deleted. This is used to track the lifecycle of
     * a transaction and determine how it should be treated in various contexts
     * (e.g., whether it should be included in reports, whether it can be
     * edited, etc.).
     *
     */
    class BaseTransaction
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
        explicit BaseTransaction(
            TransactionId              id,
            Timestamp                  timestamp,
            TransactionStatus          status,
            std::optional<std::string> comment = std::nullopt
        );
        virtual ~BaseTransaction() = default;

        [[nodiscard]] TransactionId              getId() const;
        [[nodiscard]] Timestamp                  getTimestamp() const;
        [[nodiscard]] TransactionStatus          getStatus() const;
        [[nodiscard]] std::optional<std::string> getComment() const;

        [[nodiscard]] virtual std::string toString() const;

        void setId(TransactionId id);
    };

    /**
     * @brief A class representing a financial transaction, which may involve
     * multiple accounts and cash movements. This class serves as a base for
     * more specific transaction types (e.g., deposits, withdrawals, transfers).
     *
     */
    class Transaction : public BaseTransaction
    {
       private:
        /// The cash account associated with the option transaction
        AccountId _cashAccount;
        /// The external account associated with the option transaction
        AccountId _externalAccount;

        /// The fees associated with the option transaction
        Cash _fees;

       public:
        explicit Transaction(
            TransactionId              id,
            Timestamp                  timestamp,
            TransactionStatus          status,
            AccountId                  cashAccount,
            AccountId                  externalAccount,
            Cash                       fees,
            std::optional<std::string> comment = std::nullopt
        );
        ~Transaction() override = default;

        [[nodiscard]] Cash      getFees() const;
        [[nodiscard]] AccountId getCashAccountId() const;
        [[nodiscard]] AccountId getExternalAccountId() const;

        /**
         * @brief Get the entries associated with the transaction for a given
         * external account, this is used to retrieve the specific cash
         * movements and account changes that are associated with the
         * transaction, filtered by the external account, which can be useful
         * for understanding the details of the transaction and how it affects
         * different accounts.
         *
         * @param externalAccount The external account to filter the entries by,
         * this specifies which external account's entries to retrieve, and is
         * used as a filter criterion when fetching the transaction entries.
         * @return TransactionEntries The entries associated with the
         * transaction for the specified external account, this is a collection
         * of all the individual movements and changes that are part of the
         * transaction, filtered by the given external account.
         */
        [[nodiscard]]
        virtual TransactionEntries getEntries(
            AccountId externalAccount
        ) const = 0;

       protected:
        [[nodiscard]]
        TransactionEntry _getFeeEntry(
            std::optional<AccountId> externalAccount = std::nullopt
        ) const;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_HPP__
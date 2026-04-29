#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__

#include <optional>
#include <string>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "utils/timestamp.hpp"

namespace drafts
{

    // TODO: create own draft files
    /**
     * @brief A draft representation of a transaction entry
     *
     */
    class TransactionEntryDraft
    {
       private:
        /// The ID of the account associated with the transaction entry
        AccountId _accountId;

        /// The cash amount associated with the transaction entry
        finance::Cash _cash;

        /// A flag indicating whether this transaction entry needs an external
        /// account
        bool _needsExternal = false;

       public:
        TransactionEntryDraft(AccountId accountId, finance::Cash cash);

        void setNeedsExternal(bool needsExternal);

        [[nodiscard]] AccountId getAccountId() const;

        [[nodiscard]] finance::Cash getCash() const;

        [[nodiscard]] bool needsExternal() const;
    };

    struct TransactionDataDraft
    {
    };

    /**
     * @brief A draft representation of a transaction
     *
     */
    class CreateCashTransactionDraft
    {
       private:
        /// The timestamp of the transaction
        Timestamp _timestamp;

        /// The entries of the transaction
        std::vector<TransactionEntryDraft> _entries;

        std::optional<std::string> _comment;

       public:
        CreateCashTransactionDraft(
            Timestamp                          timestamp,
            std::vector<TransactionEntryDraft> entries,
            std::optional<std::string>         comment
        );

        [[nodiscard]]
        const std::vector<TransactionEntryDraft>& getEntries() const;

        [[nodiscard]]
        const Timestamp& getTimestamp() const;

        [[nodiscard]]
        const std::optional<std::string>& getComment() const;

        void addEntry(const TransactionEntryDraft& entry);
    };

    class TransactionOverviewDraft
    {
       private:
        Timestamp _timestamp;

        std::vector<TransactionEntryDraft> _entries;

        std::optional<std::string> _comment;

       public:
        explicit TransactionOverviewDraft(
            Timestamp                          timestamp,
            std::vector<TransactionEntryDraft> entries,
            std::optional<std::string>         comment
        );

        [[nodiscard]] const Timestamp& getTimestamp() const;

        [[nodiscard]]
        const std::vector<TransactionEntryDraft>& getEntries() const;

        [[nodiscard]] const std::optional<std::string>& getComment() const;
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__
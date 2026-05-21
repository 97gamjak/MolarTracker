#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__

#include <optional>
#include <string>
#include <vector>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "utils/timestamp.hpp"

namespace drafts
{

    // TODO(97gamjak): create own draft files
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

        /// A flag indicating whether this transaction entry is for an external
        /// account
        bool _isExternal = false;

        /// The type of the transaction entry
        TransactionEntryType _type;

       public:
        TransactionEntryDraft(
            AccountId            accountId,
            finance::Cash        cash,
            TransactionEntryType type,
            bool                 isExternal
        );

        void setNeedsExternal(bool needsExternal);

        [[nodiscard]] AccountId            getAccountId() const;
        [[nodiscard]] finance::Cash        getCash() const;
        [[nodiscard]] bool                 needsExternal() const;
        [[nodiscard]] bool                 isExternal() const;
        [[nodiscard]] TransactionEntryType getType() const;
        [[nodiscard]] Currency             getCurrency() const;
    };

    /**
     * @brief A draft representation of a trade leg
     *
     */
    class TradeLegDraft
    {
       private:
        /// The ID of the account associated with the trade leg draft.
        AccountId _accountId;
        /// The unit price associated with the trade leg draft.
        finance::Cash _unitPrice;
        /// The quantity associated with the trade leg draft.
        Quantity _quantity;
        /// The ticker associated with the trade leg draft.
        std::string _ticker;
        /// The instrument ID associated with the trade leg draft.
        InstrumentId _instrumentId;
        /// The position ID associated with the trade leg draft.
        PositionId _positionId;

       public:
        TradeLegDraft(
            AccountId     accountId,
            finance::Cash unitPrice,
            Quantity      quantity,
            std::string   ticker
        );

        [[nodiscard]] AccountId          getAccountId() const;
        [[nodiscard]] finance::Cash      getUnitPrice() const;
        [[nodiscard]] Quantity           getQuantity() const;
        [[nodiscard]] const std::string& getTicker() const;
        [[nodiscard]] InstrumentId       getInstrumentId() const;
        [[nodiscard]] PositionId         getPositionId() const;
        [[nodiscard]] Currency           getCurrency() const;

        void setInstrumentId(InstrumentId instrumentId);
        void setPositionId(PositionId positionId);
    };

    /**
     * @brief A base class for a draft representation of a transaction
     *
     */
    class CreateTransactionDraft
    {
       private:
        /// The timestamp of the transaction
        Timestamp _timestamp;

        /// The entries of the transaction
        std::vector<TransactionEntryDraft> _entries;

        /// An optional comment associated with the transaction
        std::optional<std::string> _comment;

       public:
        CreateTransactionDraft(
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
        void addEntries(const std::vector<TransactionEntryDraft>& entries);
    };

    /**
     * @brief A draft representation of a transaction
     *
     */
    class CreateCashTransactionDraft : public CreateTransactionDraft
    {
       public:
        using CreateTransactionDraft::CreateTransactionDraft;
    };

    /**
     * @brief A draft representation of a stock transaction
     *
     */
    class CreateStockTransactionDraft : public CreateTransactionDraft
    {
       private:
        /// The legs of the stock transaction
        std::vector<TradeLegDraft> _legs;

       public:
        CreateStockTransactionDraft(
            Timestamp                          timestamp,
            std::vector<TransactionEntryDraft> entries,
            std::vector<TradeLegDraft>         legs,
            std::optional<std::string>         comment
        );

        [[nodiscard]] const std::vector<TradeLegDraft>& getLegs() const;

        [[nodiscard]] std::vector<TradeLegDraft>& getLegs();
    };

    /**
     * @brief A draft representation of a transaction overview, this is used
     * to display a summary of a transaction in the UI, and contains the
     * necessary information to provide an overview of the transaction
     * without needing to load the full transaction details.
     *
     */
    class TransactionOverviewDraft
    {
       private:
        /// The type of the transaction
        TransactionDataType _type;

        /// The timestamp of the transaction
        Timestamp _timestamp;

        /// The entries of the transaction
        std::vector<TransactionEntryDraft> _entries;

        /// The legs of the transaction
        std::vector<TradeLegDraft> _legs;

        /// An optional comment associated with the transaction
        std::optional<std::string> _comment;

       public:
        explicit TransactionOverviewDraft(
            TransactionDataType                type,
            Timestamp                          timestamp,
            std::vector<TransactionEntryDraft> entries,
            std::vector<TradeLegDraft>         legs,
            std::optional<std::string>         comment
        );

        [[nodiscard]] TransactionDataType               getType() const;
        [[nodiscard]] const Timestamp&                  getTimestamp() const;
        [[nodiscard]] const std::optional<std::string>& getComment() const;

        [[nodiscard]] finance::Cash getTotalGeneralCash() const;
        [[nodiscard]] finance::Cash getTotalFees() const;
        [[nodiscard]] Currency      getCurrency() const;

        [[nodiscard]] AccountId getLegAccount() const;
        [[nodiscard]] AccountId getEntryAccountId(bool includeExternal) const;

        [[nodiscard]]
        const std::vector<TransactionEntryDraft>& getEntries() const;

        [[nodiscard]]
        const std::vector<TradeLegDraft>& getLegs() const;
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__
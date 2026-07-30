#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_DRAFT_HPP__

#include <string>

#include "common/cash.hpp"
#include "common/finance.hpp"
#include "config/id_types.hpp"

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
        Cash _cash;

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
            Cash                 cash,
            TransactionEntryType type,
            bool                 isExternal
        );

        void setNeedsExternal(bool needsExternal);

        [[nodiscard]] AccountId            getAccountId() const;
        [[nodiscard]] Cash                 getCash() const;
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
        Cash _unitPrice;
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
            AccountId   accountId,
            Cash        unitPrice,
            Quantity    quantity,
            std::string ticker
        );

        [[nodiscard]] AccountId          getAccountId() const;
        [[nodiscard]] Cash               getUnitPrice() const;
        [[nodiscard]] Quantity           getQuantity() const;
        [[nodiscard]] const std::string& getTicker() const;
        [[nodiscard]] InstrumentId       getInstrumentId() const;
        [[nodiscard]] PositionId         getPositionId() const;
        [[nodiscard]] Currency           getCurrency() const;

        void setInstrumentId(InstrumentId instrumentId);
        void setPositionId(PositionId positionId);
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_DRAFT_HPP__
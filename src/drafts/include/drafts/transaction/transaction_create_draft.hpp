#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_CREATE_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_CREATE_DRAFT_HPP__

#include <optional>

#include "config/id_types.hpp"
#include "config/quantity.hpp"
#include "finance/cash.hpp"
#include "utils/timestamp.hpp"

namespace drafts
{
    /**
     * @brief Base class for creating transaction drafts.
     *
     * This class serves as a base for all transaction draft types,
     * encapsulating common properties and behaviors.
     */
    class CreateTransactionDraft
    {
       private:
        Timestamp _timestamp;

        std::optional<std::string> _comment;

       public:
        explicit CreateTransactionDraft(
            Timestamp                  timestamp,
            std::optional<std::string> comment = std::nullopt
        );

        [[nodiscard]]
        const Timestamp& getTimestamp() const;

        [[nodiscard]]
        const std::optional<std::string>& getComment() const;
    };

    /**
     * @brief Create a cash transaction draft.
     *
     * This class is used to create a draft for a cash transaction,
     * encapsulating all the necessary information to initiate the
     * transaction.
     */
    class CreateCashTransactionDraft : public CreateTransactionDraft
    {
       private:
        finance::Cash _amount;
        finance::Cash _fees;
        AccountId     _accountId;

        AccountId _externalAccount = AccountId::invalid();

       public:
        explicit CreateCashTransactionDraft(
            Timestamp                  timestamp,
            finance::Cash              amount,
            finance::Cash              fees,
            AccountId                  accountId,
            std::optional<std::string> comment = std::nullopt
        );

        void setExternalAccount(AccountId externalAccount);

        [[nodiscard]] const finance::Cash& getAmount() const;
        [[nodiscard]] const finance::Cash& getFees() const;
        [[nodiscard]] AccountId            getAccountId() const;
        [[nodiscard]] AccountId            getExternalAccount() const;
    };

    /**
     * @brief Create a stock transaction draft.
     *
     * This class is used to create a draft for a stock transaction,
     * encapsulating all the necessary information to initiate the
     * transaction.
     */
    class CreateStockTransactionDraft : public CreateTransactionDraft
    {
       private:
        std::string   _ticker;
        Quantity      _quantity;
        finance::Cash _unitPrice;
        finance::Cash _fees;

        AccountId _securityAccount;
        AccountId _cashAccount;

        InstrumentId _instrumentId    = InstrumentId::invalid();
        AccountId    _externalAccount = AccountId::invalid();
        PositionId   _positionId      = PositionId::invalid();

       public:
        explicit CreateStockTransactionDraft(
            Timestamp                  timestamp,
            std::string                ticker,
            Quantity                   quantity,
            finance::Cash              unitPrice,
            finance::Cash              fees,
            AccountId                  securityAccount,
            AccountId                  cashAccount,
            std::optional<std::string> comment = std::nullopt
        );

        void setExternalAccount(AccountId externalAccount);
        void setInstrumentId(InstrumentId instrumentId);
        void setPositionId(PositionId positionId);

        [[nodiscard]] AccountId            getSecurityAccount() const;
        [[nodiscard]] AccountId            getCashAccount() const;
        [[nodiscard]] AccountId            getExternalAccount() const;
        [[nodiscard]] InstrumentId         getInstrumentId() const;
        [[nodiscard]] std::string          getTicker() const;
        [[nodiscard]] const Quantity&      getQuantity() const;
        [[nodiscard]] const finance::Cash& getUnitPrice() const;
        [[nodiscard]] const finance::Cash& getFees() const;
        [[nodiscard]] PositionId           getPositionId() const;
    };
}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_CREATE_DRAFT_HPP__

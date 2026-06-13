#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_CREATE_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_CREATE_DRAFT_HPP__

#include <optional>

#include "config/id_types.hpp"
#include "utils/cash.hpp"
#include "utils/quantity.hpp"
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
        /// The timestamp of the transaction draft
        Timestamp _timestamp;

        /// An optional comment associated with the transaction draft
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
        /// The amount of the cash transaction
        Cash _amount;

        /// The fees associated with the cash transaction
        Cash _fees;

        /// The account ID associated with the cash transaction
        AccountId _accountId;

       public:
        explicit CreateCashTransactionDraft(
            Timestamp                  timestamp,
            Cash                       amount,
            Cash                       fees,
            AccountId                  accountId,
            std::optional<std::string> comment = std::nullopt
        );

        [[nodiscard]] const Cash& getAmount() const;
        [[nodiscard]] const Cash& getFees() const;
        [[nodiscard]] AccountId   getAccountId() const;
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
        /// The ticker symbol of the stock being transacted
        std::string _ticker;
        /// The quantity of the stock being transacted
        Quantity _quantity;
        /// The unit price of the stock being transacted
        Cash _unitPrice;
        /// The fees associated with the stock transaction
        Cash _fees;

        /// The security account ID associated with the stock transaction
        AccountId _securityAccount;
        /// The cash account ID associated with the stock transaction
        AccountId _cashAccount;

        /// The instrument ID associated with the stock transaction
        InstrumentId _instrumentId = InstrumentId::invalid();
        /// The position ID associated with the stock transaction
        PositionId _positionId = PositionId::invalid();

       public:
        explicit CreateStockTransactionDraft(
            Timestamp                  timestamp,
            std::string                ticker,
            Quantity                   quantity,
            Cash                       unitPrice,
            Cash                       fees,
            AccountId                  securityAccount,
            AccountId                  cashAccount,
            std::optional<std::string> comment = std::nullopt
        );

        void setInstrumentId(InstrumentId instrumentId);
        void setPositionId(PositionId positionId);

        [[nodiscard]] AccountId       getSecurityAccount() const;
        [[nodiscard]] AccountId       getCashAccount() const;
        [[nodiscard]] InstrumentId    getInstrumentId() const;
        [[nodiscard]] std::string     getTicker() const;
        [[nodiscard]] const Quantity& getQuantity() const;
        [[nodiscard]] const Cash&     getUnitPrice() const;
        [[nodiscard]] const Cash&     getFees() const;
        [[nodiscard]] PositionId      getPositionId() const;
    };

    /**
     * @brief Create an option transaction draft.
     *
     * This class is used to create a draft for an option transaction,
     * encapsulating all the necessary information to initiate the
     * transaction.
     */
    class CreateOptionTransactionDraft : public CreateTransactionDraft
    {
       private:
        /// The name of the option being transacted
        std::string _underlyingTicker;
        Timestamp   _expiration;
        OptionType  _optionType;
        /// The quantity of the option being transacted
        Quantity _quantity;
        /// The strike price of the option being transacted
        Cash _strikePrice;
        Cash _amount;
        /// The fees associated with the option transaction
        Cash _fees;

        std::int64_t _contractSize;

        /// The security account ID associated with the option transaction
        AccountId _securityAccount;
        /// The cash account ID associated with the option transaction
        AccountId _cashAccount;

        /// The instrument ID associated with the option transaction
        InstrumentId _instrumentId           = InstrumentId::invalid();
        InstrumentId _underlyingInstrumentId = InstrumentId::invalid();
        /// The position ID associated with the option transaction
        PositionId _positionId = PositionId::invalid();

       public:
        explicit CreateOptionTransactionDraft(
            Timestamp                  timestamp,
            std::string                ticker,
            Timestamp                  expiration,
            OptionType                 optionType,
            Quantity                   quantity,
            Cash                       amount,
            Cash                       strikePrice,
            Cash                       fees,
            std::int64_t               contractSize,
            AccountId                  securityAccount,
            AccountId                  cashAccount,
            std::optional<std::string> comment = std::nullopt
        );

        void setInstrumentId(InstrumentId instrumentId);
        void setUnderlyingInstrumentId(InstrumentId underlyingInstrumentId);
        void setPositionId(PositionId positionId);

        [[nodiscard]] AccountId          getSecurityAccount() const;
        [[nodiscard]] AccountId          getCashAccount() const;
        [[nodiscard]] InstrumentId       getInstrumentId() const;
        [[nodiscard]] InstrumentId       getUnderlyingInstrumentId() const;
        [[nodiscard]] const Quantity&    getQuantity() const;
        [[nodiscard]] const Cash&        getStrikePrice() const;
        [[nodiscard]] const Cash&        getAmount() const;
        [[nodiscard]] const Cash&        getFees() const;
        [[nodiscard]] PositionId         getPositionId() const;
        [[nodiscard]] const std::string& getUnderlyingTicker() const;
        [[nodiscard]] Timestamp          getExpiration() const;
        [[nodiscard]] OptionType         getOptionType() const;
        [[nodiscard]] std::int64_t       getContractSize() const;
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_CREATE_DRAFT_HPP__

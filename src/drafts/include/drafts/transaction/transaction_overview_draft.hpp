#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_OVERVIEW_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_OVERVIEW_DRAFT_HPP__

#include <optional>
#include <string>

#include "common/cash.hpp"
#include "common/quantity.hpp"
#include "common/timestamp.hpp"
#include "config/id_types.hpp"

namespace drafts
{

    /**
     * @brief A draft representation of a transaction overview
     *
     */
    class TransactionOverviewDraft
    {
       private:
        /// The timestamp of the transaction
        Timestamp _timestamp;

        /// An optional comment associated with the transaction
        std::optional<std::string> _comment;

       public:
        explicit TransactionOverviewDraft(
            Timestamp                  timestamp,
            std::optional<std::string> comment
        );

        [[nodiscard]] const Timestamp&                  getTimestamp() const;
        [[nodiscard]] const std::optional<std::string>& getComment() const;
    };

    /**
     * @brief A draft representation of a cash transaction overview
     *
     */
    class CashTransactionOverview : public TransactionOverviewDraft
    {
       private:
        /// The amount of the cash transaction
        Cash _amount;
        /// The fees associated with the cash transaction
        Cash _fees;

        /// The cash account ID associated with the cash transaction
        AccountId _cashAccount;
        /// The external account ID associated with the cash transaction
        AccountId _externalAccount;

       public:
        explicit CashTransactionOverview(
            Timestamp                  timestamp,
            std::optional<std::string> comment,
            Cash                       amount,
            Cash                       fees,
            AccountId                  cashAccount,
            AccountId                  externalAccount
        );

        [[nodiscard]] const Cash& getAmount() const;
        [[nodiscard]] const Cash& getFees() const;
        [[nodiscard]] AccountId   getCashAccount() const;
        [[nodiscard]] AccountId   getExternalAccount() const;
    };

    /**
     * @brief A draft representation of a stock transaction overview
     *
     */
    class StockTransactionOverview : public TransactionOverviewDraft
    {
       private:
        /// The quantity of the stock transaction
        Quantity _quantity;
        /// The unit price of the stock transaction
        Cash _unitPrice;
        /// The total fees associated with the stock transaction
        Cash _fees;

        /// The ticker symbol of the stock being transacted
        std::string _ticker;

        /// The security account ID associated with the stock transaction
        AccountId _securityAccount;
        /// The cash account ID associated with the stock transaction
        AccountId _cashAccount;

       public:
        explicit StockTransactionOverview(
            Timestamp                  timestamp,
            std::optional<std::string> comment,
            Quantity                   quantity,
            Cash                       unitPrice,
            Cash                       fees,
            std::string                ticker,
            AccountId                  securityAccount,
            AccountId                  cashAccount
        );

        [[nodiscard]] std::string getTicker() const;
        [[nodiscard]] AccountId   getSecurityAccount() const;
        [[nodiscard]] AccountId   getCashAccount() const;

        [[nodiscard]] const Quantity& getQuantity() const;
        [[nodiscard]] const Cash&     getUnitPrice() const;
        [[nodiscard]] const Cash&     getTotalFees() const;
    };
}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_OVERVIEW_DRAFT_HPP__
#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_OVERVIEW_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_OVERVIEW_DRAFT_HPP__

#include <optional>
#include <string>

#include "drafts/account_draft.hpp"
#include "utils/cash.hpp"
#include "utils/quantity.hpp"
#include "utils/timestamp.hpp"

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

        /// The cash account ID associated with the cash transaction
        AccountDraft _cashAccount;

       public:
        explicit TransactionOverviewDraft(
            Timestamp                  timestamp,
            std::optional<std::string> comment,
            AccountDraft               cashAccount
        );

        [[nodiscard]] const Timestamp&                  getTimestamp() const;
        [[nodiscard]] const std::optional<std::string>& getComment() const;
        [[nodiscard]] const AccountDraft&               getCashAccount() const;
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

       public:
        explicit CashTransactionOverview(
            Timestamp                  timestamp,
            std::optional<std::string> comment,
            Cash                       amount,
            Cash                       fees,
            AccountDraft               cashAccount
        );

        [[nodiscard]] const Cash& getAmount() const;
        [[nodiscard]] const Cash& getFees() const;
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
        AccountDraft _securityAccount;

       public:
        explicit StockTransactionOverview(
            Timestamp                  timestamp,
            std::optional<std::string> comment,
            Quantity                   quantity,
            Cash                       unitPrice,
            Cash                       fees,
            std::string                ticker,
            AccountDraft               securityAccount,
            AccountDraft               cashAccount
        );

        [[nodiscard]] const std::string&  getTicker() const;
        [[nodiscard]] const AccountDraft& getSecurityAccount() const;

        [[nodiscard]] const Quantity& getQuantity() const;
        [[nodiscard]] const Cash&     getUnitPrice() const;
        [[nodiscard]] const Cash&     getTotalFees() const;
    };
}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION__TRANSACTION_OVERVIEW_DRAFT_HPP__
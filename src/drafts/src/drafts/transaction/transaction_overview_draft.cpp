#include "drafts/transaction/transaction_overview_draft.hpp"

#include <utility>

namespace drafts
{

    /**
     * @brief Construct a new Transaction Overview Draft:: Transaction Overview
     * Draft object
     *
     * @param timestamp
     * @param comment
     * @param cashAccount
     */
    TransactionOverviewDraft::TransactionOverviewDraft(
        Timestamp                  timestamp,
        std::optional<std::string> comment,
        AccountDraft               cashAccount
    )
        : _timestamp(timestamp),
          _comment(std::move(comment)),
          _cashAccount(std::move(cashAccount))
    {
    }

    /**
     * @brief Gets the timestamp of the transaction overview draft.
     *
     * @return const Timestamp& The timestamp of the transaction overview draft.
     */
    const Timestamp& TransactionOverviewDraft::getTimestamp() const
    {
        return _timestamp;
    }

    /**
     * @brief Gets the comment of the transaction overview draft.
     *
     * @return const std::optional<std::string>& The comment of the transaction
     * overview draft.
     */
    const std::optional<std::string>& TransactionOverviewDraft::getComment(
    ) const
    {
        return _comment;
    }

    /**
     * @brief Gets the cash account of the transaction overview draft.
     *
     * @return const AccountDraft& The cash account of the transaction overview
     * draft.
     */
    const AccountDraft& TransactionOverviewDraft::getCashAccount() const
    {
        return _cashAccount;
    }

    /**
     * @brief Construct a new Cash Transaction Overview:: Cash Transaction
     * Overview object
     *
     * @param timestamp
     * @param comment
     * @param amount
     * @param fees
     * @param cashAccount
     * @param externalAccount
     */
    CashTransactionOverview::CashTransactionOverview(
        Timestamp                  timestamp,
        std::optional<std::string> comment,
        Cash                       amount,
        Cash                       fees,
        AccountDraft               cashAccount
    )
        : TransactionOverviewDraft(
              timestamp,
              std::move(comment),
              std::move(cashAccount)
          ),
          _amount(amount),
          _fees(fees)
    {
    }

    /**
     * @brief Gets the amount of the cash transaction overview draft.
     *
     * @return const Cash& The amount of the cash transaction overview
     * draft.
     */
    const Cash& CashTransactionOverview::getAmount() const { return _amount; }

    /**
     * @brief Gets the fees of the cash transaction overview draft.
     *
     * @return const Cash& The fees of the cash transaction overview
     * draft.
     */
    const Cash& CashTransactionOverview::getFees() const { return _fees; }

    /**
     * @brief Construct a new Stock Transaction Overview:: Stock Transaction
     * Overview object
     *
     * @param timestamp
     * @param comment
     * @param quantity
     * @param unitPrice
     * @param fees
     * @param ticker
     * @param securityAccount
     * @param cashAccount
     */
    StockTransactionOverview::StockTransactionOverview(
        Timestamp                  timestamp,
        std::optional<std::string> comment,
        Quantity                   quantity,
        Cash                       unitPrice,
        Cash                       fees,
        std::string                ticker,
        AccountDraft               securityAccount,
        AccountDraft               cashAccount
    )
        : TransactionOverviewDraft(
              timestamp,
              std::move(comment),
              std::move(cashAccount)
          ),
          _quantity(quantity),
          _unitPrice(unitPrice),
          _fees(fees),
          _ticker(std::move(ticker)),
          _securityAccount(std::move(securityAccount))
    {
    }

    /**
     * @brief Gets the ticker of the stock transaction overview draft.
     *
     * @return std::string The ticker of the stock transaction overview draft.
     */
    const std::string& StockTransactionOverview::getTicker() const
    {
        return _ticker;
    }

    /**
     * @brief Gets the security account ID of the stock transaction overview
     * draft.
     *
     * @return const AccountDraft& The security account of the stock transaction
     * overview draft.
     */
    const AccountDraft& StockTransactionOverview::getSecurityAccount() const
    {
        return _securityAccount;
    }

    /**
     * @brief Gets the quantity of the stock transaction overview draft.
     *
     * @return const Quantity& The quantity of the stock transaction overview
     * draft.
     */
    const Quantity& StockTransactionOverview::getQuantity() const
    {
        return _quantity;
    }

    /**
     * @brief Gets the unit price of the stock transaction overview draft.
     *
     * @return const Cash& The unit price of the stock transaction
     * overview draft.
     */
    const Cash& StockTransactionOverview::getUnitPrice() const
    {
        return _unitPrice;
    }

    /**
     * @brief Gets the total fees of the stock transaction overview draft.
     *
     * @return const Cash& The total fees of the stock transaction
     * overview draft.
     */
    const Cash& StockTransactionOverview::getTotalFees() const { return _fees; }

}   // namespace drafts

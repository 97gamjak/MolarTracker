#include "drafts/transaction/transaction_overview_draft.hpp"

namespace drafts
{

    /**
     * @brief Construct a new Transaction Overview Draft:: Transaction Overview
     * Draft object
     *
     * @param timestamp
     * @param comment
     */
    TransactionOverviewDraft::TransactionOverviewDraft(
        Timestamp                  timestamp,
        std::optional<std::string> comment
    )
        : _timestamp(timestamp), _comment(std::move(comment))
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

    CashTransactionOverview::CashTransactionOverview(
        Timestamp                  timestamp,
        std::optional<std::string> comment,
        finance::Cash              amount,
        finance::Cash              fees,
        AccountId                  cashAccount,
        AccountId                  externalAccount
    )
        : TransactionOverviewDraft(timestamp, std::move(comment)),
          _amount(amount),
          _fees(fees),
          _cashAccount(cashAccount),
          _externalAccount(externalAccount)
    {
    }

    const finance::Cash& CashTransactionOverview::getAmount() const
    {
        return _amount;
    }

    const finance::Cash& CashTransactionOverview::getFees() const
    {
        return _fees;
    }

    AccountId CashTransactionOverview::getCashAccount() const
    {
        return _cashAccount;
    }

    AccountId CashTransactionOverview::getExternalAccount() const
    {
        return _externalAccount;
    }

    StockTransactionOverview::StockTransactionOverview(
        Timestamp                  timestamp,
        std::optional<std::string> comment,
        Quantity                   quantity,
        finance::Cash              unitPrice,
        finance::Cash              fees,
        std::string                ticker,
        AccountId                  securityAccount,
        AccountId                  cashAccount
    )
        : TransactionOverviewDraft(timestamp, std::move(comment)),
          _quantity(quantity),
          _unitPrice(unitPrice),
          _fees(fees),
          _ticker(std::move(ticker)),
          _securityAccount(securityAccount),
          _cashAccount(cashAccount)
    {
    }

    std::string StockTransactionOverview::getTicker() const { return _ticker; }

    AccountId StockTransactionOverview::getSecurityAccount() const
    {
        return _securityAccount;
    }

    AccountId StockTransactionOverview::getCashAccount() const
    {
        return _cashAccount;
    }

    const Quantity& StockTransactionOverview::getQuantity() const
    {
        return _quantity;
    }

    const finance::Cash& StockTransactionOverview::getUnitPrice() const
    {
        return _unitPrice;
    }

    const finance::Cash& StockTransactionOverview::getTotalFees() const
    {
        return _fees;
    }
}   // namespace drafts

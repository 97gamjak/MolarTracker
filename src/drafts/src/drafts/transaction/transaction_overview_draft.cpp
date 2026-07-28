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
    const std::optional<std::string>& TransactionOverviewDraft::
        getComment() const
    {
        return _comment;
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
     * @brief Gets the cash account ID of the cash transaction overview draft.
     *
     * @return AccountId The cash account ID of the cash transaction overview
     * draft.
     */
    AccountId CashTransactionOverview::getCashAccount() const
    {
        return _cashAccount;
    }

    /**
     * @brief Gets the external account ID of the cash transaction overview
     * draft.
     *
     * @return AccountId The external account ID of the cash transaction
     * overview draft.
     */
    AccountId CashTransactionOverview::getExternalAccount() const
    {
        return _externalAccount;
    }

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

    /**
     * @brief Gets the ticker of the stock transaction overview draft.
     *
     * @return std::string The ticker of the stock transaction overview draft.
     */
    std::string StockTransactionOverview::getTicker() const { return _ticker; }

    /**
     * @brief Gets the security account ID of the stock transaction overview
     * draft.
     *
     * @return AccountId The security account ID of the stock transaction
     * overview draft.
     */
    AccountId StockTransactionOverview::getSecurityAccount() const
    {
        return _securityAccount;
    }

    /**
     * @brief Gets the cash account ID of the stock transaction overview draft.
     *
     * @return AccountId The cash account ID of the stock transaction overview
     * draft.
     */
    AccountId StockTransactionOverview::getCashAccount() const
    {
        return _cashAccount;
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

    /**
     * @brief Construct a new Option Transaction Overview:: Option Transaction
     * Overview object
     *
     * @param timestamp
     * @param comment
     * @param quantity
     * @param premium
     * @param fees
     * @param optionName
     * @param buySell
     * @param action
     * @param securityAccount
     * @param cashAccount
     */
    OptionTransactionOverview::OptionTransactionOverview(
        Timestamp                  timestamp,
        std::optional<std::string> comment,
        Quantity                   quantity,
        Cash                       premium,
        Cash                       fees,
        std::string                optionName,
        OptionBuySell              buySell,
        TransactionOptionAction    action,
        AccountId                  securityAccount,
        AccountId                  cashAccount
    )
        : TransactionOverviewDraft(timestamp, std::move(comment)),
          _quantity(quantity),
          _premium(premium),
          _fees(fees),
          _optionName(std::move(optionName)),
          _buySell(buySell),
          _action(action),
          _securityAccount(securityAccount),
          _cashAccount(cashAccount)
    {
    }

    /**
     * @brief Gets the display name of the option transaction overview draft.
     *
     * @return std::string The display name of the option transaction overview
     * draft.
     */
    std::string OptionTransactionOverview::getOptionName() const
    {
        return _optionName;
    }

    /**
     * @brief Gets the security account ID of the option transaction overview
     * draft.
     *
     * @return AccountId The security account ID of the option transaction
     * overview draft.
     */
    AccountId OptionTransactionOverview::getSecurityAccount() const
    {
        return _securityAccount;
    }

    /**
     * @brief Gets the cash account ID of the option transaction overview draft.
     *
     * @return AccountId The cash account ID of the option transaction overview
     * draft.
     */
    AccountId OptionTransactionOverview::getCashAccount() const
    {
        return _cashAccount;
    }

    /**
     * @brief Gets the quantity of the option transaction overview draft.
     *
     * @return const Quantity& The quantity of the option transaction overview
     * draft.
     */
    const Quantity& OptionTransactionOverview::getQuantity() const
    {
        return _quantity;
    }

    /**
     * @brief Gets the premium of the option transaction overview draft.
     *
     * @return const Cash& The premium of the option transaction overview
     * draft.
     */
    const Cash& OptionTransactionOverview::getPremium() const
    {
        return _premium;
    }

    /**
     * @brief Gets the total fees of the option transaction overview draft.
     *
     * @return const Cash& The total fees of the option transaction overview
     * draft.
     */
    const Cash& OptionTransactionOverview::getTotalFees() const
    {
        return _fees;
    }

    /**
     * @brief Gets the buy/sell direction of the option transaction overview
     * draft.
     *
     * @return OptionBuySell The buy/sell direction of the option transaction
     * overview draft.
     */
    OptionBuySell OptionTransactionOverview::getBuySell() const
    {
        return _buySell;
    }

    /**
     * @brief Gets the action of the option transaction overview draft.
     *
     * @return TransactionOptionAction The action of the option transaction
     * overview draft.
     */
    TransactionOptionAction OptionTransactionOverview::getAction() const
    {
        return _action;
    }

}   // namespace drafts

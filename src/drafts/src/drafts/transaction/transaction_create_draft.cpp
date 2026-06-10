#include "drafts/transaction/transaction_create_draft.hpp"

namespace drafts
{
    /**
     * @brief Create a Transaction Draft:: Create Transaction Draft object
     *
     * @param timestamp
     * @param comment
     */
    CreateTransactionDraft::CreateTransactionDraft(
        Timestamp                  timestamp,
        std::optional<std::string> comment
    )
        : _timestamp(timestamp), _comment(std::move(comment))
    {
    }

    /**
     * @brief Gets the timestamp of the transaction draft.
     *
     * @return const Timestamp&
     */
    const Timestamp& CreateTransactionDraft::getTimestamp() const
    {
        return _timestamp;
    }

    /**
     * @brief Gets the comment of the transaction draft.
     *
     * @return const std::optional<std::string>&
     */
    const std::optional<std::string>& CreateTransactionDraft::getComment() const
    {
        return _comment;
    }

    /**
     * @brief Create a Cash Transaction Draft:: Create Cash Transaction Draft
     * object
     *
     * @param timestamp
     * @param amount
     * @param fees
     * @param accountId
     * @param comment
     */
    CreateCashTransactionDraft::CreateCashTransactionDraft(
        Timestamp                  timestamp,
        finance::Cash              amount,
        finance::Cash              fees,
        AccountId                  accountId,
        std::optional<std::string> comment
    )
        : CreateTransactionDraft(timestamp, std::move(comment)),
          _amount(amount),
          _fees(fees),
          _accountId(accountId)
    {
    }

    /**
     * @brief Set the external account for the cash transaction draft.
     *
     * @param externalAccount The ID of the external account to associate with
     * the cash transaction draft
     */
    void CreateCashTransactionDraft::setExternalAccount(
        AccountId externalAccount
    )
    {
        _externalAccount = externalAccount;
    }

    /**
     * @brief Gets the amount of the cash transaction draft.
     *
     * @return const finance::Cash& The amount of the cash transaction draft.
     */
    const finance::Cash& CreateCashTransactionDraft::getAmount() const
    {
        return _amount;
    }

    /**
     * @brief Gets the fees of the cash transaction draft.
     *
     * @return const finance::Cash& The fees of the cash transaction draft.
     */
    const finance::Cash& CreateCashTransactionDraft::getFees() const
    {
        return _fees;
    }

    /**
     * @brief Gets the account ID of the cash transaction draft.
     *
     * @return AccountId The account ID of the cash transaction draft.
     */
    AccountId CreateCashTransactionDraft::getAccountId() const
    {
        return _accountId;
    }

    /**
     * @brief Gets the external account ID of the cash transaction draft.
     *
     * @return AccountId The external account ID of the cash transaction draft.
     */
    AccountId CreateCashTransactionDraft::getExternalAccount() const
    {
        return _externalAccount;
    }

    /**
     * @brief Construct a new Stock Transaction Draft:: Stock Transaction Draft
     * object
     *
     * @param timestamp
     * @param ticker
     * @param quantity
     * @param unitPrice
     * @param fees
     * @param securityAccount
     * @param cashAccount
     * @param comment
     */
    CreateStockTransactionDraft::CreateStockTransactionDraft(
        Timestamp                  timestamp,
        std::string                ticker,
        Quantity                   quantity,
        finance::Cash              unitPrice,
        finance::Cash              fees,
        AccountId                  securityAccount,
        AccountId                  cashAccount,
        std::optional<std::string> comment
    )
        : CreateTransactionDraft(timestamp, std::move(comment)),
          _ticker(std::move(ticker)),
          _quantity(quantity),
          _unitPrice(unitPrice),
          _fees(fees),
          _securityAccount(securityAccount),
          _cashAccount(cashAccount)
    {
    }

    /**
     * @brief Set the external account for the stock transaction draft.
     *
     * @param externalAccount The ID of the external account to associate with
     * the stock transaction draft
     */
    void CreateStockTransactionDraft::setExternalAccount(
        AccountId externalAccount
    )
    {
        _externalAccount = externalAccount;
    }

    /**
     * @brief Set the instrument ID for the stock transaction draft.
     *
     * @param instrumentId The ID of the instrument to associate with the stock
     * transaction draft
     */
    void CreateStockTransactionDraft::setInstrumentId(InstrumentId instrumentId)
    {
        _instrumentId = instrumentId;
    }

    /**
     * @brief Set the position ID for the stock transaction draft.
     *
     * @param positionId The ID of the position to associate with the stock
     * transaction draft
     */
    void CreateStockTransactionDraft::setPositionId(PositionId positionId)
    {
        _positionId = positionId;
    }

    /**
     * @brief Gets the security account ID of the stock transaction draft.
     *
     * @return AccountId The security account ID of the stock transaction draft.
     */
    AccountId CreateStockTransactionDraft::getSecurityAccount() const
    {
        return _securityAccount;
    }

    /**
     * @brief Gets the cash account ID of the stock transaction draft.
     *
     * @return AccountId The cash account ID of the stock transaction draft.
     */
    AccountId CreateStockTransactionDraft::getCashAccount() const
    {
        return _cashAccount;
    }

    /**
     * @brief Gets the external account ID of the stock transaction draft.
     *
     * @return AccountId The external account ID of the stock transaction draft.
     */
    AccountId CreateStockTransactionDraft::getExternalAccount() const
    {
        return _externalAccount;
    }

    /**
     * @brief Gets the instrument ID of the stock transaction draft.
     *
     * @return InstrumentId The instrument ID of the stock transaction draft.
     */
    InstrumentId CreateStockTransactionDraft::getInstrumentId() const
    {
        return _instrumentId;
    }

    /**
     * @brief Gets the position ID of the stock transaction draft.
     *
     * @return PositionId The position ID of the stock transaction draft.
     */
    std::string CreateStockTransactionDraft::getTicker() const
    {
        return _ticker;
    }

    /**
     * @brief Gets the quantity of the stock transaction draft.
     *
     * @return const Quantity& The quantity of the stock transaction draft.
     */
    const Quantity& CreateStockTransactionDraft::getQuantity() const
    {
        return _quantity;
    }

    /**
     * @brief Gets the unit price of the stock transaction draft.
     *
     * @return const finance::Cash& The unit price of the stock transaction
     * draft.
     */
    const finance::Cash& CreateStockTransactionDraft::getUnitPrice() const
    {
        return _unitPrice;
    }

    /**
     * @brief Gets the fees of the stock transaction draft.
     *
     * @return const finance::Cash& The fees of the stock transaction draft.
     */
    const finance::Cash& CreateStockTransactionDraft::getFees() const
    {
        return _fees;
    }

    /**
     * @brief Gets the position ID of the stock transaction draft.
     *
     * @return PositionId The position ID of the stock transaction draft.
     */
    PositionId CreateStockTransactionDraft::getPositionId() const
    {
        return _positionId;
    }
}   // namespace drafts
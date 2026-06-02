#include "drafts/transaction/transaction_create_draft.hpp"

namespace drafts
{
    CreateTransactionDraft::CreateTransactionDraft(
        Timestamp                  timestamp,
        std::optional<std::string> comment
    )
        : _timestamp(timestamp), _comment(std::move(comment))
    {
    }

    const Timestamp& CreateTransactionDraft::getTimestamp() const
    {
        return _timestamp;
    }

    const std::optional<std::string>& CreateTransactionDraft::getComment() const
    {
        return _comment;
    }

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

    void CreateCashTransactionDraft::setExternalAccount(
        AccountId externalAccount
    )
    {
        _externalAccount = externalAccount;
    }

    const finance::Cash& CreateCashTransactionDraft::getAmount() const
    {
        return _amount;
    }

    const finance::Cash& CreateCashTransactionDraft::getFees() const
    {
        return _fees;
    }

    AccountId CreateCashTransactionDraft::getAccountId() const
    {
        return _accountId;
    }

    AccountId CreateCashTransactionDraft::getExternalAccount() const
    {
        return _externalAccount;
    }

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

    void CreateStockTransactionDraft::setExternalAccount(
        AccountId externalAccount
    )
    {
        _externalAccount = externalAccount;
    }

    void CreateStockTransactionDraft::setInstrumentId(InstrumentId instrumentId)
    {
        _instrumentId = instrumentId;
    }

    void CreateStockTransactionDraft::setPositionId(PositionId positionId)
    {
        _positionId = positionId;
    }

    AccountId CreateStockTransactionDraft::getSecurityAccount() const
    {
        return _securityAccount;
    }

    AccountId CreateStockTransactionDraft::getCashAccount() const
    {
        return _cashAccount;
    }

    AccountId CreateStockTransactionDraft::getExternalAccount() const
    {
        return _externalAccount;
    }

    InstrumentId CreateStockTransactionDraft::getInstrumentId() const
    {
        return _instrumentId;
    }

    std::string CreateStockTransactionDraft::getTicker() const
    {
        return _ticker;
    }

    const Quantity& CreateStockTransactionDraft::getQuantity() const
    {
        return _quantity;
    }

    const finance::Cash& CreateStockTransactionDraft::getUnitPrice() const
    {
        return _unitPrice;
    }
    const finance::Cash& CreateStockTransactionDraft::getFees() const
    {
        return _fees;
    }
    PositionId CreateStockTransactionDraft::getPositionId() const
    {
        return _positionId;
    }
}   // namespace drafts
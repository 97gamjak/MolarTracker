#include "finance/transaction/stock_transaction.hpp"

#include "config/id_types.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "finance/transaction/transaction_entry.hpp"

namespace finance
{
    StockTransaction::StockTransaction(
        TransactionId              id,
        Timestamp                  timestamp,
        TransactionStatus          status,
        InstrumentId               instrumentId,
        AccountId                  securityAccount,
        AccountId                  cashAccount,
        AccountId                  externalAccount,
        Quantity                   quantity,
        Cash                       unitPrice,
        Cash                       fees,
        PositionId                 positionId,
        std::optional<std::string> comment
    )
        : Transaction(id, timestamp, status, std::move(comment)),
          _instrumentId(instrumentId),
          _securityAccount(securityAccount),
          _cashAccount(cashAccount),
          _externalAccount(externalAccount),
          _quantity(quantity),
          _unitPrice(unitPrice),
          _fees(fees),
          _positionId(positionId)
    {
    }

    InstrumentId StockTransaction::getInstrumentId() const
    {
        return _instrumentId;
    }

    AccountId StockTransaction::getSecurityAccountId() const
    {
        return _securityAccount;
    }

    AccountId StockTransaction::getCashAccountId() const
    {
        return _cashAccount;
    }

    AccountId StockTransaction::getExternalAccountId() const
    {
        return _externalAccount;
    }

    Quantity StockTransaction::getQuantity() const { return _quantity; }

    Cash StockTransaction::getUnitPrice() const { return _unitPrice; }

    Cash StockTransaction::getFees() const { return _fees; }

    PositionId StockTransaction::getPositionId() const { return _positionId; }

    TransactionEntries StockTransaction::getTransactionEntries() const
    {
        TransactionEntries entries;

        entries.addTransactionEntry(_getAmountEntry());
        entries.addTransactionEntry(_getFeeEntry(false));
        entries.addTransactionEntry(_getFeeEntry(true));

        return entries;
    }

    TradeData StockTransaction::getTradeData() const
    {
        const TradeLeg leg{
            _securityAccount,
            _instrumentId,
            _quantity,
            _unitPrice,
            _positionId
        };
        return TradeData{{leg}};
    }

    Cash StockTransaction::getAmount() const { return _quantity * _unitPrice; }

    TransactionEntry StockTransaction::_getAmountEntry() const
    {
        return TransactionEntry{
            TransactionEntryId::invalid(),
            _cashAccount,
            -getAmount(),
            TransactionEntryType::General
        };
    }

    TransactionEntry StockTransaction::_getFeeEntry(bool external) const
    {
        return TransactionEntry{
            TransactionEntryId::invalid(),
            external ? _cashAccount : _externalAccount,
            external ? -_fees : _fees,
            TransactionEntryType::Fees
        };
    }
}   // namespace finance
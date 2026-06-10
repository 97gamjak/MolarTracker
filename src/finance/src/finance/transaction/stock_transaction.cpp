#include "finance/transaction/stock_transaction.hpp"

#include "config/id_types.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "finance/transaction/transaction_entry.hpp"

namespace finance
{
    /**
     * @brief Construct a new Stock Transaction:: Stock Transaction object
     *
     * @param id
     * @param timestamp
     * @param status
     * @param instrumentId
     * @param securityAccount
     * @param cashAccount
     * @param externalAccount
     * @param quantity
     * @param unitPrice
     * @param fees
     * @param positionId
     * @param comment
     */
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

    /**
     * @brief Get the security account associated with the stock transaction
     *
     * @return AccountId
     */
    AccountId StockTransaction::getSecurityAccountId() const
    {
        return _securityAccount;
    }

    /**
     * @brief Get the cash account associated with the stock transaction
     *
     * @return AccountId
     */
    AccountId StockTransaction::getCashAccountId() const
    {
        return _cashAccount;
    }

    /**
     * @brief Get the external account associated with the stock transaction
     *
     * @return AccountId
     */
    AccountId StockTransaction::getExternalAccountId() const
    {
        return _externalAccount;
    }

    /**
     * @brief Get the quantity of the stock transaction
     *
     * @return const Quantity&
     */
    const Quantity& StockTransaction::getQuantity() const { return _quantity; }

    /**
     * @brief Get the unit price of the stock transaction
     *
     * @return const Cash&
     */
    const Cash& StockTransaction::getUnitPrice() const { return _unitPrice; }

    /**
     * @brief Get the fees associated with the stock transaction
     *
     * @return const Cash&
     */
    const Cash& StockTransaction::getFees() const { return _fees; }

    /**
     * @brief Get the position ID associated with the stock transaction
     *
     * @return PositionId
     */
    PositionId StockTransaction::getPositionId() const { return _positionId; }

    /**
     * @brief Get the transaction entries associated with the stock transaction,
     * this will return a list of transaction entries that represent the cash
     * flows associated with the stock transaction, including the amount of the
     * transaction and any fees.
     *
     * @return TransactionEntries
     */
    TransactionEntries StockTransaction::getTransactionEntries() const
    {
        TransactionEntries entries;

        entries.add(_getAmountEntry());

        if (_externalAccount.isValid())
        {
            entries.add(_getFeeEntry(false));
            entries.add(_getFeeEntry(true));
        }

        return entries;
    }

    /**
     * @brief Get the trade data associated with the stock transaction, this
     * will return a TradeData object that represents the details of the trade
     * associated with the stock transaction, including the security account,
     * instrument, quantity, unit price, and position ID.
     *
     * @return TradeData
     */
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

    /**
     * @brief Get the total amount of the stock transaction, this will calculate
     * the total amount of the transaction based on the quantity and unit price,
     * which represents the cash flow associated with the stock transaction.
     *
     * @return Cash
     */
    Cash StockTransaction::getAmount() const { return _quantity * _unitPrice; }

    /**
     * @brief Get the base instrument ID associated with the stock transaction,
     * this will return the instrument ID of the stock that is being traded in
     * the transaction, which is used for categorizing and displaying the
     * transaction in the transaction overview.
     *
     * @return InstrumentId
     */
    InstrumentId StockTransaction::getBaseInstrumentId() const
    {
        return _instrumentId;
    }

    /**
     * @brief Get the transaction entry representing the amount of the stock
     * transaction, this will create a TransactionEntry that represents the cash
     * flow of the stock transaction based on the quantity and unit price.
     *
     * @return TransactionEntry
     */
    TransactionEntry StockTransaction::_getAmountEntry() const
    {
        return TransactionEntry{
            TransactionEntryId::invalid(),
            _cashAccount,
            -getAmount(),
            TransactionEntryType::General
        };
    }

    /**
     * @brief Get the transaction entry representing the fees of the stock
     * transaction, this will create a TransactionEntry that represents the fees
     * associated with the stock transaction, which may involve both the cash
     * account and an external account if applicable.
     *
     * @param external A flag indicating whether to create the fee entry for the
     * external account (true) or the cash account (false).
     * @return TransactionEntry
     */
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
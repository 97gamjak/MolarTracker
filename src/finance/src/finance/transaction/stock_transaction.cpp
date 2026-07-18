#include "finance/transaction/stock_transaction.hpp"

#include "config/id_types.hpp"
#include "finance/transaction/security_transaction.hpp"
#include "finance/transaction/stock_data.hpp"
#include "finance/transaction/trade_leg.hpp"
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
        : SecurityTransaction(
              id,
              timestamp,
              status,
              instrumentId,
              securityAccount,
              cashAccount,
              externalAccount,
              quantity,
              fees,
              positionId,
              std::move(comment)
          ),
          _unitPrice(unitPrice)
    {
    }

    /**
     * @brief Get the unit price of the stock transaction
     *
     * @return const Cash&
     */
    const Cash& StockTransaction::getUnitPrice() const { return _unitPrice; }

    /**
     * @brief Get the transaction entries associated with the stock transaction,
     * this will return a list of transaction entries that represent the cash
     * flows associated with the stock transaction, including the amount of the
     * transaction and any fees.
     *
     * @param externalAccount The external account ID to use for the fee entry,
     * this allows the fee to be associated with a specific external account for
     * reporting and categorization purposes.
     *
     * @return TransactionEntries
     */
    TransactionEntries StockTransaction::getEntries(
        AccountId externalAccount
    ) const
    {
        TransactionEntries entries;

        entries.add(_getAmountEntry());

        entries.add(_getFeeEntry());
        entries.add(_getFeeEntry(externalAccount));

        return entries;
    }

    /**
     * @brief Get the stock data associated with the stock transaction, this
     * will return a StockData object that represents the details of the stock
     * transaction, including the security account, instrument, quantity, unit
     * price, and position ID.
     *
     * @return StockData
     */
    StockData StockTransaction::getStockData() const
    {
        const TradeLeg leg{
            getSecurityAccountId(),
            getInstrumentId(),
            getQuantity(),
            getUnitPrice(),
            getPositionId()
        };
        return StockData{{leg}};
    }

    /**
     * @brief Get the total amount of the stock transaction, this will calculate
     * the total amount of the transaction based on the quantity and unit price,
     * which represents the cash flow associated with the stock transaction.
     *
     * @return Cash
     */
    Cash StockTransaction::getAmount() const
    {
        return getQuantity() * getUnitPrice();
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
            getCashAccountId(),
            -getAmount(),
            TransactionEntryType::General
        };
    }

}   // namespace finance
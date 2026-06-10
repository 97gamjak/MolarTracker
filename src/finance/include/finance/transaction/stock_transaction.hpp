#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__STOCK_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__STOCK_TRANSACTION_HPP__

#include "config/id_types.hpp"
#include "config/quantity.hpp"
#include "finance/cash.hpp"
#include "finance/transaction/i_security_transaction.hpp"
#include "finance/transaction/trade_data.hpp"
#include "finance/transaction/transaction.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "finance/transaction/transaction_entry.hpp"

namespace finance
{
    /**
     * @brief A stock transaction represents a financial transaction involving
     * stocks.
     *
     */
    class StockTransaction : public Transaction, ISecurityTransaction
    {
       private:
        /// The instrument ID of the stock being traded in the transaction
        InstrumentId _instrumentId;

        /// The security account associated with the stock transaction
        AccountId _securityAccount;
        /// The cash account associated with the stock transaction
        AccountId _cashAccount;
        /// The external account associated with the stock transaction
        AccountId _externalAccount;

        /// The quantity of the stock being traded in the transaction
        Quantity _quantity;
        /// The unit price of the stock being traded in the transaction
        Cash _unitPrice;
        /// The fees associated with the stock transaction
        Cash _fees;

        /// The position ID associated with the stock transaction
        PositionId _positionId;

       public:
        StockTransaction(
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
            std::optional<std::string> comment = std::nullopt
        );

        [[nodiscard]] AccountId       getSecurityAccountId() const;
        [[nodiscard]] AccountId       getCashAccountId() const;
        [[nodiscard]] AccountId       getExternalAccountId() const;
        [[nodiscard]] const Quantity& getQuantity() const override;
        [[nodiscard]] const Cash&     getUnitPrice() const;
        [[nodiscard]] const Cash&     getFees() const;
        [[nodiscard]] PositionId      getPositionId() const;

        [[nodiscard]]
        TransactionEntries getTransactionEntries() const;

        [[nodiscard]] TradeData getTradeData() const;

        [[nodiscard]] Cash getAmount() const;

        [[nodiscard]] InstrumentId getBaseInstrumentId() const override;

       private:
        [[nodiscard]] TransactionEntry _getAmountEntry() const;
        [[nodiscard]] TransactionEntry _getFeeEntry(bool external) const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__STOCK_TRANSACTION_HPP__

#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__STOCK_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__STOCK_TRANSACTION_HPP__

#include "config/id_types.hpp"
#include "config/quantity.hpp"
#include "finance/cash.hpp"
#include "finance/transaction/trade_data.hpp"
#include "finance/transaction/transaction.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "finance/transaction/transaction_entry.hpp"

namespace finance
{
    class StockTransaction : public Transaction
    {
       private:
        InstrumentId _instrumentId;

        AccountId _securityAccount;
        AccountId _cashAccount;
        AccountId _externalAccount;

        Quantity _quantity;

        Cash _unitPrice;

        Cash _fees;

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

        [[nodiscard]] InstrumentId getInstrumentId() const;
        [[nodiscard]] AccountId    getSecurityAccountId() const;
        [[nodiscard]] AccountId    getCashAccountId() const;
        [[nodiscard]] AccountId    getExternalAccountId() const;
        [[nodiscard]] Quantity     getQuantity() const;
        [[nodiscard]] Cash         getUnitPrice() const;
        [[nodiscard]] Cash         getFees() const;
        [[nodiscard]] PositionId   getPositionId() const;

        [[nodiscard]]
        TransactionEntries getTransactionEntries() const;

        [[nodiscard]] TradeData getTradeData() const;

        [[nodiscard]] Cash getAmount() const;

       private:
        [[nodiscard]] TransactionEntry _getAmountEntry() const;
        [[nodiscard]] TransactionEntry _getFeeEntry(bool external) const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__STOCK_TRANSACTION_HPP__

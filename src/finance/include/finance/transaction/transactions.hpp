#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__

#include <vector>

#include "config/id_types.hpp"
#include "config/strong_id.hpp"
#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"

namespace finance
{
    class Accounts;

    class ISecurityTransactions
    {
       public:
        virtual ~ISecurityTransactions() = default;

        [[nodiscard]] virtual idSet<InstrumentId> getBaseInstrumentIds(
        ) const = 0;

        [[nodiscard]] virtual Quantity getTotalQuantity() const = 0;
    };

    class StockTransactions : public Vector<StockTransaction>,
                              public ISecurityTransactions
    {
       public:
        [[nodiscard]] idSet<InstrumentId> getBaseInstrumentIds() const override;
        [[nodiscard]] Quantity            getTotalQuantity() const override;
    };

    class CashTransactions : public Vector<CashTransaction>
    {
       public:
    };

    class SecurityView : public ISecurityTransactions
    {
       private:
        const StockTransactions& _stockTransactions;

       public:
        explicit SecurityView(const StockTransactions& stockTransactions);
        ~SecurityView() override = default;

        // delete copy and move constructors
        SecurityView(const SecurityView&)            = delete;
        SecurityView(SecurityView&&)                 = delete;
        SecurityView& operator=(const SecurityView&) = delete;
        SecurityView& operator=(SecurityView&&)      = delete;

        [[nodiscard]] idSet<InstrumentId> getBaseInstrumentIds() const override;
        [[nodiscard]] Quantity            getTotalQuantity() const override;
    };

    class Transactions
    {
       private:
        CashTransactions  _cashTransactions;
        StockTransactions _stockTransactions;

       public:
        void addTransactions(
            const std::vector<DomainTransaction>& transactions,
            const Accounts&                       accounts
        );

        [[nodiscard]] const CashTransactions& cash() const;

        [[nodiscard]] const StockTransactions& stocks() const;

        [[nodiscard]] SecurityView securities() const;

        [[nodiscard]] bool empty() const;

       private:
        [[nodiscard]] std::vector<const Transaction*> _getTransactions() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__

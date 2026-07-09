#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__

#include <vector>

#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"

namespace finance
{
    class Accounts;

    /**
     * @brief Interface for managing security-related transactions.
     *
     */
    class ISecurityTransactions
    {
       public:
        virtual ~ISecurityTransactions() = default;

        /**
         * @brief Get the Base Instrument Ids
         *
         * @return idSet<InstrumentId>
         */
        [[nodiscard]]
        virtual idSet<InstrumentId> getBaseInstrumentIds() const = 0;
    };

    /**
     * @brief Class for managing stock transactions.
     *
     */
    class StockTransactions : public Vector<StockTransaction>,
                              public ISecurityTransactions
    {
       public:
        void sort();

        [[nodiscard]] idSet<InstrumentId> getBaseInstrumentIds() const override;
    };

    /**
     * @brief Class for managing cash transactions.
     *
     */
    class CashTransactions : public Vector<CashTransaction>
    {
       public:
    };

    /**
     * @brief Class for managing security transactions.
     *
     */
    class SecurityView : public ISecurityTransactions
    {
       private:
        /// The stock transactions that are part of the security view
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
    };

    /**
     * @brief Class for managing all transactions.
     *
     */
    class Transactions
    {
       private:
        /// The list of cash transactions
        CashTransactions _cashTransactions;
        /// The list of stock transactions
        StockTransactions _stockTransactions;

       public:
        Transactions() = default;
        Transactions(
            const std::vector<DomainTransaction>& transactions,
            const Accounts&                       accounts
        );
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

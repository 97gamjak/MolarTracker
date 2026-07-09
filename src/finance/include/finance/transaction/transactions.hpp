#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__

#include <memory>
#include <vector>

#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "utils/container/id_map.hpp"

namespace finance
{
    class AccountsView;   // forward declaration

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
         * @return IdSet<InstrumentId>
         */
        [[nodiscard]]
        virtual IdSet<InstrumentId> getBaseInstrumentIds() const = 0;
    };

    /**
     * @brief Class for managing stock transactions.
     *
     */
    class StockTransactionsView : public Vector<StockTransactionView>,
                                  public ISecurityTransactions
    {
       public:
        void sort();

        [[nodiscard]] IdSet<InstrumentId> getBaseInstrumentIds() const override;
    };

    /**
     * @brief Class for managing cash transactions.
     *
     */
    class CashTransactionsView : public Vector<CashTransactionView>
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
        const StockTransactionsView& _stockTransactions;

       public:
        explicit SecurityView(const StockTransactionsView& stockTransactions);
        ~SecurityView() override = default;

        // delete copy and move constructors
        SecurityView(const SecurityView&)            = delete;
        SecurityView(SecurityView&&)                 = delete;
        SecurityView& operator=(const SecurityView&) = delete;
        SecurityView& operator=(SecurityView&&)      = delete;

        [[nodiscard]] IdSet<InstrumentId> getBaseInstrumentIds() const override;
    };

    /**
     * @brief Class for managing all transactions.
     *
     */
    class Transactions
    {
       private:
        /// The list of cash transactions
        CashTransactionsView _cashTransactions;
        /// The list of stock transactions
        StockTransactionsView _stockTransactions;

       public:
        Transactions() = default;
        Transactions(
            const std::vector<DomainTransaction>& transactions,
            const AccountsView&                   accounts
        );
        void addTransactions(
            const std::vector<DomainTransaction>& transactions,
            const AccountsView&                   accounts
        );

        [[nodiscard]] SecurityView securities() const;

        [[nodiscard]] bool empty() const;

       private:
        [[nodiscard]] std::vector<const Transaction*> _getTransactions() const;
    };

    class TransactionsView
    {
        IdObjectMap<StockTransactionView> _stockTransactions;
        IdObjectMap<CashTransactionView>  _cashTransactions;

       public:
        explicit TransactionsView(
            const std::vector<std::shared_ptr<const DomainTransaction>>&
                                transactions,
            const AccountsView& accounts
        );

        [[nodiscard]] const CashTransactionsView&  cash() const;
        [[nodiscard]] const StockTransactionsView& stocks() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__

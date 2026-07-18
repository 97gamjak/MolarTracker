#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__

#include <memory>
#include <vector>

#include "error/finance_error.hpp"
#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/option_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "utils/container/id_map.hpp"
#include "utils/container/set.hpp"

namespace finance
{
    class Accounts;   // forward declaration
    class Options;    // forward declaration

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
    class StockTransactions : public Vector<StockTransaction>,
                              public ISecurityTransactions
    {
       public:
        void sort();

        [[nodiscard]]
        IdSet<InstrumentId> getBaseInstrumentIds() const override;
    };

    /**
     * @brief Class for managing option transactions.
     *
     */
    class OptionTransactions : public Vector<OptionTransaction>,
                               public ISecurityTransactions
    {
       public:
        void sort();

        [[nodiscard]]
        IdSet<InstrumentId> getBaseInstrumentIds() const override;

        [[nodiscard]]
        std::vector<TransactionId> getIds() const;
    };

    /**
     * @brief Class for managing cash transactions.
     *
     */
    class CashTransactions : public Vector<CashTransaction>
    {
       public:
        void sort();
    };

    /**
     * @brief Class for managing security transactions.
     *
     */
    class SecurityView : public ISecurityTransactions
    {
       private:
        /// The stock transactions that are part of the security view
        StockTransactions _stockTransactions;

        /// The option transactions that are part of the security view
        OptionTransactions _optionTransactions;

       public:
        explicit SecurityView(
            StockTransactions  stockTransactions,
            OptionTransactions optionTransactions
        );
        ~SecurityView() override = default;

        // delete copy and move constructors
        SecurityView(const SecurityView&)            = delete;
        SecurityView(SecurityView&&)                 = delete;
        SecurityView& operator=(const SecurityView&) = delete;
        SecurityView& operator=(SecurityView&&)      = delete;

        [[nodiscard]]
        IdSet<InstrumentId> getBaseInstrumentIds() const override;
    };

    /**
     * @brief Class for managing all transactions.
     *
     */
    class Transactions
    {
       private:
        class TransactionsImpl;
        std::shared_ptr<TransactionsImpl> _impl;

       public:
        Transactions();
        Transactions(
            const std::vector<DomainTransaction>& transactions,
            const Accounts&                       accounts
        );
        Transactions(
            const CashTransactions&   cash,
            const StockTransactions&  stock,
            const OptionTransactions& options
        );
        ~Transactions();

        [[nodiscard]]
        FinanceResult<void> addTransactions(
            const std::vector<DomainTransaction>& transactions,
            const Accounts&                       accounts
        );

        void addTransaction(const StockTransaction&);
        void addTransaction(const OptionTransaction&);

        [[nodiscard]]
        const CashTransactions& cash() const;
        [[nodiscard]]
        const StockTransactions& stocks() const;
        [[nodiscard]]
        const OptionTransactions& options() const;
        [[nodiscard]]
        SecurityView securities() const;

        [[nodiscard]] bool empty() const;
        [[nodiscard]] bool containsOptions() const;

        [[nodiscard]]
        IdMap<PositionId, Transactions> groupByPosition() const;

        [[nodiscard]]
        Transactions filter(const IdSet<AccountId>& accountIds) const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__

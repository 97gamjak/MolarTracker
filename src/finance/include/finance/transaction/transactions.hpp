#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__

#include <vector>

#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/option_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "utils/container/set.hpp"

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

        [[nodiscard]] IdSet<InstrumentId> getBaseInstrumentIds() const override;
    };

    class OptionTransactions : public Vector<OptionTransaction>,
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

        /// The option transactions that are part of the security view
        const OptionTransactions& _optionTransactions;

       public:
        explicit SecurityView(
            const StockTransactions&  stockTransactions,
            const OptionTransactions& optionTransactions
        );
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
        CashTransactions _cashTransactions;
        /// The list of stock transactions
        StockTransactions _stockTransactions;
        /// The list of option transactions
        OptionTransactions _optionTransactions;

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

        [[nodiscard]] const OptionTransactions& options() const;

        [[nodiscard]] SecurityView securities() const;

        [[nodiscard]] bool empty() const;

       private:
        [[nodiscard]] std::vector<const Transaction*> _getTransactions() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__

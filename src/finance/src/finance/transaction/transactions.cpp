#include "finance/transaction/transactions.hpp"

#include "finance/account/accounts.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "finance/transaction/transaction_converter.hpp"

namespace finance
{
    /**
     * @brief sort the stock transactions by timestamp in ascending order
     *
     */
    void StockTransactions::sort()
    {
        std::ranges::sort(
            getItems(),
            [](const StockTransaction& txA, const StockTransaction& txB)
            { return txA.getTimestamp() < txB.getTimestamp(); }
        );
    }

    /**
     * @brief Get the Base Instrument Ids from the stock transactions
     *
     * @return idSet<InstrumentId>
     */
    idSet<InstrumentId> StockTransactions::getBaseInstrumentIds() const
    {
        idSet<InstrumentId> instrumentIds;
        for (const auto& transaction : getItems())
            instrumentIds.insert(transaction.getBaseInstrumentId());

        return instrumentIds;
    }

    /**
     * @brief Construct a new Security View:: Security View object
     *
     * @param transactions
     */
    SecurityView::SecurityView(const StockTransactions& transactions)
        : _stockTransactions(transactions)
    {
    }

    /**
     * @brief Get the Base Instrument Ids from the security view
     *
     * @return idSet<InstrumentId>
     */
    idSet<InstrumentId> SecurityView::getBaseInstrumentIds() const
    {
        return _stockTransactions.getBaseInstrumentIds();
    }

    /**
     * @brief Construct a new Transactions:: Transactions object
     *
     * @param transactions
     * @param accounts
     */
    Transactions::Transactions(
        const std::vector<DomainTransaction>& transactions,
        const Accounts&                       accounts
    )
    {
        addTransactions(transactions, accounts);
    }

    /**
     * @brief Add transactions to the Transactions object, this will take a list
     * of domain transactions and convert them to their respective transaction
     * types and add them to the appropriate transaction lists.
     *
     * @param transactions
     * @param accounts
     */
    void Transactions::addTransactions(
        const std::vector<DomainTransaction>& transactions,
        const Accounts&                       accounts
    )
    {
        for (const auto& transaction : transactions)
        {
            switch (transaction.getType())
            {
                // TODO: fix this here
                case TransactionDataType::Cash:
                    _cashTransactions.add(
                        TransactionConverter::toCash(transaction, accounts)
                            .value()
                    );
                    break;
                // TODO: make Trade to a Stock transaction type and more
                case TransactionDataType::Stock:
                    _stockTransactions.add(
                        TransactionConverter::toStock(transaction, accounts)
                            .value()
                    );
                    break;
            }
        }
    }

    /**
     * @brief Get the list of cash transactions
     *
     * @return const CashTransactions&
     */
    const CashTransactions& Transactions::cash() const
    {
        return _cashTransactions;
    }

    /**
     * @brief Get the list of stock transactions
     *
     * @return const StockTransactions&
     */
    const StockTransactions& Transactions::stocks() const
    {
        return _stockTransactions;
    }

    /**
     * @brief Get the security view of the transactions, this will return a
     * SecurityView object that provides access to the stock transactions and
     * their associated base instrument IDs for display in the transaction
     * overview.
     *
     * @return SecurityView
     */
    SecurityView Transactions::securities() const
    {
        return SecurityView(_stockTransactions);
    }

    /**
     * @brief Check if there are no transactions in the Transactions object
     *
     * @return true if there are no transactions, false otherwise
     */
    bool Transactions::empty() const { return _getTransactions().empty(); }

    /**
     * @brief Get a list of pointers to all transactions, this will combine the
     * cash and stock transactions into a single list of pointers for easy
     * iteration and access to all transactions in the Transactions object.
     *
     * @return std::vector<const Transaction*>
     */
    std::vector<const Transaction*> Transactions::_getTransactions() const
    {
        std::vector<const Transaction*> transactions;
        transactions.reserve(
            _cashTransactions.size() + _stockTransactions.size()
        );
        for (const auto& transaction : _cashTransactions)
        {
            transactions.push_back(
                dynamic_cast<const Transaction*>(&transaction)
            );
        }
        for (const auto& transaction : _stockTransactions)
        {
            transactions.push_back(
                dynamic_cast<const Transaction*>(&transaction)
            );
        }
        return transactions;
    }
}   // namespace finance
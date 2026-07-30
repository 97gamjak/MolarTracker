#include "finance/transaction/transactions.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "common/container/set.hpp"
#include "error/finance_error.hpp"
#include "finance/account/accounts.hpp"
#include "finance/transaction/option_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "finance/transaction/transaction_converter.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("Finance.Transactions");

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
     * @brief sort the option transactions by timestamp in ascending order
     *
     */
    void OptionTransactions::sort()
    {
        std::ranges::sort(
            getItems(),
            [](const OptionTransaction& txA, const OptionTransaction& txB)
            { return txA.getTimestamp() < txB.getTimestamp(); }
        );
    }

    /**
     * @brief Get the Base Instrument Ids from the stock transactions
     *
     * @return IdSet<InstrumentId>
     */
    IdSet<InstrumentId> StockTransactions::getBaseInstrumentIds() const
    {
        IdSet<InstrumentId> instrumentIds;
        for (const auto& transaction : getItems())
            instrumentIds.insert(transaction.getInstrumentId());

        return instrumentIds;
    }

    /**
     * @brief Get the Base Instrument Ids from the option transactions
     *
     * @return IdSet<InstrumentId>
     */
    IdSet<InstrumentId> OptionTransactions::getBaseInstrumentIds() const
    {
        IdSet<InstrumentId> instrumentIds;
        for (const auto& transaction : getItems())
            instrumentIds.insert(transaction.getInstrumentId());

        return instrumentIds;
    }

    /**
     * @brief Get the IDs of the option transactions, this will return a vector
     * of TransactionId objects representing the IDs of the option transactions
     * managed by the OptionTransactions class.
     *
     * @return std::vector<TransactionId>
     */
    std::vector<TransactionId> OptionTransactions::getIds() const
    {
        std::vector<TransactionId> ids;
        for (const auto& transaction : getItems())
            ids.push_back(transaction.getId());

        return ids;
    }

    /**
     * @brief Construct a new Security View:: Security View object
     *
     * @param stockTransactions
     * @param optionTransactions
     */
    SecurityView::SecurityView(
        StockTransactions  stockTransactions,
        OptionTransactions optionTransactions
    )
        : _stockTransactions(std::move(stockTransactions)),
          _optionTransactions(std::move(optionTransactions))
    {
    }

    /**
     * @brief Get the Base Instrument Ids from the security view
     *
     * @return IdSet<InstrumentId>
     */
    IdSet<InstrumentId> SecurityView::getBaseInstrumentIds() const
    {
        auto instrumentIds = _stockTransactions.getBaseInstrumentIds();

        IdSet<InstrumentId> ids;
        for (const auto& tx : _optionTransactions)
            ids.insert(tx.getInstrumentId());
        instrumentIds.combine(ids);
        return instrumentIds;
    }

    /**
     * @brief Implementation class for Transactions, encapsulating the details
     * of managing different types of financial transactions.
     */
    class Transactions::TransactionsImpl
    {
       private:
        /// The list of cash transactions
        CashTransactions _cashTransactions;
        /// The list of stock transactions
        StockTransactions _stockTransactions;
        /// The list of option transactions
        OptionTransactions _optionTransactions;

       public:
        void add(const OptionTransaction& tx);
        void add(const CashTransaction& tx);
        void add(const StockTransaction& tx);

        [[nodiscard]] const CashTransactions&   cash() const;
        [[nodiscard]] const StockTransactions&  stocks() const;
        [[nodiscard]] const OptionTransactions& options() const;
        [[nodiscard]] SecurityView              securities() const;

        [[nodiscard]] std::vector<const Transaction*> getTransactions() const;
    };

    /**
     * @brief Get the security view of the transactions, this will return a
     * SecurityView object that provides access to the stock transactions
     * and their associated base instrument IDs for display in the
     * transaction overview.
     *
     * @return SecurityView
     */
    SecurityView Transactions::TransactionsImpl::securities() const
    {
        return SecurityView(_stockTransactions, _optionTransactions);
    }

    /**
     * @brief add an option transaction to the transactions implementation, this
     * will add the provided option transaction to the list of option
     * transactions managed by the implementation.
     * @param tx The option transaction to add.
     */
    void Transactions::TransactionsImpl::add(const OptionTransaction& tx)
    {
        _optionTransactions.add(tx);
    }

    /**
     * @brief add an option transaction to the transactions implementation, this
     * will add the provided option transaction to the list of option
     * transactions managed by the implementation.
     * @param tx The option transaction to add.
     */
    void Transactions::addTransaction(const OptionTransaction& tx)
    {
        _impl->add(tx);
    }

    /**
     * @brief add a stock transaction to the transactions implementation, this
     * will add the provided stock transaction to the list of stock transactions
     * managed by the implementation.
     * @param tx The stock transaction to add.
     */
    void Transactions::addTransaction(const StockTransaction& tx)
    {
        _impl->add(tx);
    }

    /**
     * @brief add a cash transaction to the transactions implementation, this
     * will add the provided cash transaction to the list of cash transactions
     * managed by the implementation.
     * @param tx The cash transaction to add.
     */
    void Transactions::TransactionsImpl::add(const CashTransaction& tx)
    {
        _cashTransactions.add(tx);
    }

    /**
     * @brief add a stock transaction to the transactions implementation, this
     * will add the provided stock transaction to the list of stock transactions
     * managed by the implementation.
     * @param tx The stock transaction to add.
     */
    void Transactions::TransactionsImpl::add(const StockTransaction& tx)
    {
        _stockTransactions.add(tx);
    }

    /**
     * @brief Construct a new Transactions:: Transactions object
     */
    Transactions::Transactions() : _impl(std::make_shared<TransactionsImpl>())
    {
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
        : _impl(std::make_shared<TransactionsImpl>())
    {
        const auto error = addTransactions(transactions, accounts);

        if (!error)
            LOG_ERROR(error.error().toString());
    }

    /**
     * @brief Construct a new Transactions:: Transactions object
     *
     * @param cash
     * @param stocks
     * @param options
     */
    Transactions::Transactions(
        const CashTransactions&   cash,
        const StockTransactions&  stocks,
        const OptionTransactions& options
    )
        : _impl(std::make_shared<TransactionsImpl>())
    {
        for (const auto& tx : cash)
            _impl->add(tx);

        for (const auto& tx : stocks)
            _impl->add(tx);

        for (const auto& tx : options)
            _impl->add(tx);
    }

    Transactions::~Transactions() = default;

    /**
     * @brief Add transactions to the Transactions object, this will take a list
     * of domain transactions and convert them to their respective transaction
     * types and add them to the appropriate transaction lists.
     *
     * @param transactions
     * @param accounts
     * @return FinanceResult<void> if all transactions were added successfully,
     * or an error if any transaction failed to convert or add.
     */
    FinanceResult<void> Transactions::addTransactions(
        const std::vector<DomainTransaction>& transactions,
        const Accounts&                       accounts
    )
    {
        for (const auto& transaction : transactions)
        {
            switch (transaction.getType())
            {
                case TransactionDataType::Cash:
                {
                    const auto cashTx =
                        TransactionConverter::toCash(transaction, accounts);
                    if (!cashTx)
                    {
                        const auto newError = cashTx.error().convert(
                            FinanceErrorType::InvalidTransaction,
                            "Failed to convert transaction with ID " +
                                transaction.getId().toString() +
                                " to cash transaction"
                        );
                        LOG_ERROR(newError.toString());
                        return newError;
                    }
                    _impl->add(cashTx.value());
                    break;
                }
                case TransactionDataType::Stock:
                {
                    const auto stockTx =
                        TransactionConverter::toStock(transaction);
                    if (!stockTx)
                    {
                        const auto error = stockTx.error().convert(
                            FinanceErrorType::InvalidTransaction,
                            "Failed to convert transaction with ID " +
                                transaction.getId().toString() +
                                " to stock transaction"
                        );
                        LOG_ERROR(error.toString());
                        return error;
                    }
                    _impl->add(stockTx.value());
                    break;
                }
                case TransactionDataType::Option:
                {
                    const auto optionTx =
                        TransactionConverter::toOption(transaction);
                    if (!optionTx)
                    {
                        const auto error = optionTx.error().convert(
                            FinanceErrorType::InvalidTransaction,
                            "Failed to convert transaction with ID " +
                                transaction.getId().toString() +
                                " to option transaction"
                        );
                        LOG_ERROR(error.toString());
                        return error;
                    }
                    _impl->add(optionTx.value());
                    break;
                }
            }
        }

        return FinanceResult<void>{};
    }

    /**
     * @brief Get the list of cash transactions
     *
     * @return const CashTransactions&
     */
    const CashTransactions& Transactions::TransactionsImpl::cash() const
    {
        return _cashTransactions;
    }

    /**
     * @brief Get the list of cash transactions
     *
     * @return const CashTransactions&
     */
    const CashTransactions& Transactions::cash() const { return _impl->cash(); }

    /**
     * @brief Get the list of stock transactions
     *
     * @return const StockTransactions&
     */
    const StockTransactions& Transactions::TransactionsImpl::stocks() const
    {
        return _stockTransactions;
    }

    /**
     * @brief Get the list of stock transactions
     *
     * @return const StockTransactions&
     */
    const StockTransactions& Transactions::stocks() const
    {
        return _impl->stocks();
    }

    /**
     * @brief Get the list of option transactions
     *
     * @return OptionTransactions
     */
    const OptionTransactions& Transactions::TransactionsImpl::options() const
    {
        return _optionTransactions;
    }

    /**
     * @brief Get the list of option transactions
     *
     * @return OptionTransactions
     */
    const OptionTransactions& Transactions::options() const
    {
        return _impl->options();
    }

    /**
     * @brief Get the security view of the transactions, this will return a
     * SecurityView object that provides access to the stock transactions
     * and their associated base instrument IDs for display in the
     * transaction overview.
     *
     * @return SecurityView
     */
    SecurityView Transactions::securities() const
    {
        return _impl->securities();
    }

    /**
     * @brief Check if there are no transactions in the Transactions object
     *
     * @return true if there are no transactions, false otherwise
     */
    bool Transactions::empty() const
    {
        return _impl->getTransactions().empty();
    }

    /**
     * @brief Check if there are any option transactions in the Transactions
     * object
     *
     * @return true if there are option transactions, false otherwise
     */
    bool Transactions::containsOptions() const
    {
        return !_impl->options().empty();
    }

    /**
     * @brief Get a list of pointers to all transactions, this will combine
     * the cash and stock transactions into a single list of pointers for
     * easy iteration and access to all transactions in the Transactions
     * object.
     *
     * @return std::vector<const Transaction*>
     */
    std::vector<const Transaction*> Transactions::TransactionsImpl::
        getTransactions() const
    {
        std::vector<const Transaction*> transactions;
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
        for (const auto& transaction : _optionTransactions)
        {
            transactions.push_back(
                dynamic_cast<const Transaction*>(&transaction)
            );
        }
        return transactions;
    }

    /**
     * @brief Group the transactions by position ID, this will create a map of
     * PositionId to Transactions, where each entry in the map contains all
     * transactions associated with that position ID.
     *
     * @return IdMap<PositionId, Transactions>
     */
    IdMap<PositionId, Transactions> Transactions::groupByPosition() const
    {
        IdMap<PositionId, Transactions> positionMap;

        for (const auto& transaction : stocks())
        {
            const auto positionId = transaction.getPositionId();
            if (positionId.isValid())
            {
                if (!positionMap.contains(positionId))
                    positionMap.addUnchecked(positionId, Transactions());

                positionMap.at(positionId).addTransaction(transaction);
            }
        }

        for (const auto& transaction : options())
        {
            const auto positionId = transaction.getPositionId();
            if (positionId.isValid())
            {
                if (!positionMap.contains(positionId))
                    positionMap.addUnchecked(positionId, Transactions());

                positionMap.at(positionId).addTransaction(transaction);
            }
        }

        return positionMap;
    }

    /**
     * @brief Filter the transactions by a set of account IDs, this will return
     * a new Transactions object containing only the transactions that involve
     * any of the specified account IDs.
     *
     * @param accountIds The set of account IDs to filter by.
     * @return Transactions The filtered transactions.
     */
    Transactions Transactions::filter(const IdSet<AccountId>& accountIds) const
    {
        CashTransactions   cashTx;
        StockTransactions  stockTx;
        OptionTransactions optionTx;
        for (const auto& tx : _impl->getTransactions())
        {
            const auto involvedAccounts = tx->getInvolvedAccounts();
            if (involvedAccounts.intersects(accountIds))
            {
                switch (tx->getTransactionType())
                {
                    case TransactionDataType::Cash:
                    {
                        const auto* txPtr =
                            dynamic_cast<const CashTransaction*>(tx);
                        if (txPtr != nullptr)
                            cashTx.add(*txPtr);
                        break;
                    }
                    case TransactionDataType::Stock:
                    {
                        const auto* txPtr =
                            dynamic_cast<const StockTransaction*>(tx);
                        if (txPtr != nullptr)
                            stockTx.add(*txPtr);
                        break;
                    }
                    case TransactionDataType::Option:
                    {
                        const auto* txPtr =
                            dynamic_cast<const OptionTransaction*>(tx);
                        if (txPtr != nullptr)
                            optionTx.add(*txPtr);
                        break;
                    }
                }
            }
        }

        return {cashTx, stockTx, optionTx};
    }

    /**
     * @brief Get the instrument IDs of the stock transactions, this will return
     * a set of InstrumentId objects representing the unique instruments
     * involved in the stock transactions managed by the Transactions object.
     *
     * @return IdSet<InstrumentId>
     */
    IdSet<InstrumentId> Transactions::getStockInstrumentIds() const
    {
        return stocks().getBaseInstrumentIds();
    }

    /**
     * @brief Get the instrument IDs of the option transactions, this will
     * return a set of InstrumentId objects representing the unique instruments
     * involved in the option transactions managed by the Transactions object.
     *
     * @return IdSet<InstrumentId>
     */
    IdSet<InstrumentId> Transactions::getOptionInstrumentIds() const
    {
        return options().getBaseInstrumentIds();
    }

}   // namespace finance
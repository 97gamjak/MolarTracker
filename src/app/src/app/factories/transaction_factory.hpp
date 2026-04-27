#ifndef __APP__SRC__APP__FACTORIES__TRANSACTION_FACTORY_HPP__
#define __APP__SRC__APP__FACTORIES__TRANSACTION_FACTORY_HPP__

#include "finance/cash_transaction.hpp"
#include "finance/transaction.hpp"
#include "finance/transaction_entry.hpp"
#include "sql_models/instrument_row.hpp"
#include "sql_models/transaction_entry_row.hpp"
#include "sql_models/transaction_row.hpp"

namespace app
{

    /**
     * @brief A factory class for creating and converting transaction-related
     * objects
     *
     */
    class TransactionFactory
    {
       public:
        [[nodiscard]]
        static TransactionRow toRow(const finance::Transaction& transaction);

        [[nodiscard]]
        static finance::Transaction fromRow(const TransactionRow& row);

        [[nodiscard]]
        static TransactionEntryRow toEntryRow(
            const finance::TransactionEntry& entry,
            TransactionId                    transactionId
        );

        [[nodiscard]]
        static finance::TransactionEntry fromEntryRow(
            const TransactionEntryRow& row
        );
    };

}   // namespace app

#endif   // __APP__SRC__APP__FACTORIES__TRANSACTION_FACTORY_HPP__

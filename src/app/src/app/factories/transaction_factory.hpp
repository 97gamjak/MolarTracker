#ifndef __APP__SRC__APP__FACTORIES__TRANSACTION_FACTORY_HPP__
#define __APP__SRC__APP__FACTORIES__TRANSACTION_FACTORY_HPP__

#include "finance/transaction.hpp"
#include "finance/transaction_entry.hpp"
#include "sql_models/instrument_row.hpp"
#include "sql_models/transaction_entry_row.hpp"
#include "sql_models/transaction_row.hpp"

namespace app
{

    class TransactionFactory
    {
       public:
        static TransactionRow toTransactionRow(
            const finance::Transaction& transaction
        );

        static finance::Transaction fromTransactionRow(
            const TransactionRow& row
        );

        static TransactionEntryRow toTransactionEntryRow(
            const finance::TransactionEntry& entry,
            TransactionId                    transactionId,
            InstrumentId                     instrumentId
        );

        static InstrumentRow toInstrumentRow(
            const finance::TransactionDetail& detail
        );
    };

}   // namespace app

#endif   // __APP__SRC__APP__FACTORIES__TRANSACTION_FACTORY_HPP__

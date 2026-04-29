#ifndef __APP__SRC__APP__FACTORIES__TRANSACTION_FACTORY_HPP__
#define __APP__SRC__APP__FACTORIES__TRANSACTION_FACTORY_HPP__

#include "config/id_types.hpp"

namespace finance
{
    class Transaction;
    class TransactionEntry;
}   // namespace finance

struct TransactionRow;        // forward declaration
struct TransactionEntryRow;   // forward declaration

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

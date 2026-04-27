#ifndef __APP__SRC__APP__FACTORIES__INSTRUMENT_FACTORY_HPP__
#define __APP__SRC__APP__FACTORIES__INSTRUMENT_FACTORY_HPP__

#include "config/finance.hpp"
#include "finance/transaction_entry.hpp"
#include "sql_models/instrument_row.hpp"

namespace app
{

    /**
     * @brief A factory class for creating and converting instrument-related
     * objects
     *
     */
    class InstrumentFactory
    {
       public:
        [[nodiscard]]
        static InstrumentRow toRow(const finance::TransactionDetail& detail);

        [[nodiscard]]
        static finance::TransactionDetail fromRow(
            const InstrumentRow& row,
            micro_units          amount
        );
    };

}   // namespace app

#endif   // __APP__SRC__APP__FACTORIES__INSTRUMENT_FACTORY_HPP__

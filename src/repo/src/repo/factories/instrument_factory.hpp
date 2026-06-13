#ifndef __REPO__SRC__REPO__FACTORIES__INSTRUMENT_FACTORY_HPP__
#define __REPO__SRC__REPO__FACTORIES__INSTRUMENT_FACTORY_HPP__

#include <utility>

#include "sql_models/option_row.hpp"

namespace finance
{
    class Stock;    // Forward declaration
    class Option;   // Forward declaration
}   // namespace finance

struct InstrumentRow;
struct StockRow;
struct OptionRow;

namespace repo
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
        static std::pair<InstrumentRow, StockRow> fromStock(
            const finance::Stock& stock
        );

        [[nodiscard]]
        static finance::Stock toStock(const StockRow& row);

        [[nodiscard]]
        static std::pair<InstrumentRow, OptionRow> fromOption(
            const finance::Option& option
        );
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__FACTORIES__INSTRUMENT_FACTORY_HPP__

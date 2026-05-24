#ifndef __REPO__SRC__REPO__FACTORIES__INSTRUMENT_FACTORY_HPP__
#define __REPO__SRC__REPO__FACTORIES__INSTRUMENT_FACTORY_HPP__

#include <utility>

namespace finance
{
    class Stock;   // Forward declaration
}   // namespace finance

struct InstrumentRow;
struct StockRow;

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
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__FACTORIES__INSTRUMENT_FACTORY_HPP__

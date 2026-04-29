#ifndef __APP__SRC__APP__FACTORIES__INSTRUMENT_FACTORY_HPP__
#define __APP__SRC__APP__FACTORIES__INSTRUMENT_FACTORY_HPP__

#include <utility>

namespace finance
{
    class Stock;   // Forward declaration
}   // namespace finance

struct InstrumentRow;
struct StockRow;

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
        static std::pair<InstrumentRow, StockRow> fromStock(
            const finance::Stock& stock
        );
    };

}   // namespace app

#endif   // __APP__SRC__APP__FACTORIES__INSTRUMENT_FACTORY_HPP__

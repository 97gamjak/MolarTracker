#include "instrument_factory.hpp"

#include "finance/stock.hpp"
#include "sql_models/instrument_row.hpp"
#include "sql_models/stock_row.hpp"

namespace app
{
    /**
     * @brief Create an InstrumentRow and StockRow from a Stock object, this
     * factory method takes a Stock object as input and creates corresponding
     * InstrumentRow and StockRow objects that can be inserted into the
     * database, ensuring that the relationships between the tables are
     * maintained correctly (e.g., the instrumentId in the StockRow matches the
     * id in the InstrumentRow).
     *
     * @param stock
     * @return std::pair<InstrumentRow, StockRow>
     */
    std::pair<InstrumentRow, StockRow> InstrumentFactory::fromStock(
        const finance::Stock& stock
    )
    {
        InstrumentRow instrumentRow;
        instrumentRow.currency = stock.getCurrency();
        instrumentRow.id       = stock.getInstrumentId();

        StockRow stockRow;
        stockRow.ticker       = stock.getTicker();
        stockRow.id           = stock.getStockId();
        stockRow.instrumentId = stock.getInstrumentId();

        return {instrumentRow, stockRow};
    }

}   // namespace app

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
        instrumentRow.id = stock.getInstrumentId();

        StockRow stockRow;
        stockRow.ticker       = stock.getTicker();
        stockRow.id           = stock.getId();
        stockRow.instrumentId = stock.getInstrumentId();
        stockRow.currency     = stock.getCurrency();

        return {instrumentRow, stockRow};
    }

    /**
     * @brief Create a Stock object from a StockRow, this factory method takes a
     * StockRow object as input and creates a corresponding Stock object that
     * can be used in the application, ensuring that the details from the
     * database row are correctly mapped to the properties of the Stock object.
     *
     * @param row
     * @return finance::Stock
     */
    finance::Stock InstrumentFactory::toStock(const StockRow& row)
    {
        auto stock = finance::Stock(row.ticker.value(), row.currency.value());

        stock.setId(row.id.value());
        stock.setInstrumentId(row.instrumentId.value());

        return stock;
    }

}   // namespace app

#include "instrument_factory.hpp"

#include <string>

#include "finance/instrument/option.hpp"
#include "finance/instrument/stock.hpp"
#include "sql_models/instrument_row.hpp"
#include "sql_models/option_row.hpp"
#include "sql_models/stock_row.hpp"

namespace repo
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
        stockRow.shortName    = stock.getShortName();
        stockRow.longName     = stock.getLongName();
        stockRow.assetClass   = stock.getAssetClass();
        stockRow.sector       = stock.getSector();
        stockRow.industry     = stock.getIndustry();
        stockRow.exchange     = stock.getExchange();

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
        auto stock = finance::Stock(
            row.ticker.value(),
            row.currency.value(),
            row.shortName.value(),
            row.longName.value(),
            row.exchange.value(),
            row.industry.value(),
            row.sector.value(),
            row.assetClass.value()
        );

        stock.setId(row.id.value());
        stock.setInstrumentId(row.instrumentId.value());

        return stock;
    }

    std::pair<InstrumentRow, OptionRow> InstrumentFactory::fromOption(
        const finance::Option& option
    )
    {
        InstrumentRow instrumentRow;
        instrumentRow.id = option.getInstrumentId();

        OptionRow optionRow;
        optionRow.id           = option.getId();
        optionRow.instrumentId = option.getInstrumentId();
        optionRow.underlyingInstrumentId =
            option.getUnderlying().getInstrumentId();
        optionRow.strikePrice    = option.getStrikePrice().getAmount();
        optionRow.currency       = option.getStrikePrice().getCurrency();
        optionRow.expirationDate = option.getExpirationDate();
        optionRow.optionType     = option.getOptionType();
        optionRow.contractSize   = option.getContractSize();

        return {instrumentRow, optionRow};
    }

    finance::Option InstrumentFactory::toOption(
        const OptionRow& row,
        const StockRow&  stockRow
    )
    {
        return finance::Option{
            row.id.value(),
            row.instrumentId.value(),
            toStock(stockRow),
            row.optionType.value(),
            finance::Cash{row.currency.value(), row.strikePrice.value()},
            row.expirationDate.value(),
            row.contractSize.value()
        };
    }

}   // namespace repo

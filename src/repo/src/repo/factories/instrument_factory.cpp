#include "instrument_factory.hpp"

#include <string>

#include "finance/instrument/instrument_predicates.hpp"
#include "finance/instrument/option.hpp"
#include "finance/instrument/stock.hpp"
#include "orm/query_options.hpp"
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

    /**
     * @brief Create an InstrumentRow and OptionRow from an Option object, this
     * factory method takes an Option object as input and creates corresponding
     * InstrumentRow and OptionRow objects that can be inserted into the
     * database, ensuring that the relationships between the tables are
     * maintained correctly (e.g., the instrumentId in the OptionRow matches the
     * id in the InstrumentRow).
     *
     * @param option
     * @return std::pair<InstrumentRow, OptionRow>
     */
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

    /**
     * @brief Create an Option object from an OptionRow and its corresponding
     * StockRow, this factory method takes an OptionRow and its associated
     * StockRow as input and creates a corresponding Option object that can be
     * used in the application, ensuring that the details from the database rows
     * are correctly mapped to the properties of the Option object, and that
     * the underlying stock information is included in the Option object.
     *
     * @param row
     * @param stockRow
     * @return finance::Option
     */
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
            Cash{row.currency.value(), row.strikePrice.value()},
            row.expirationDate.value(),
            row.contractSize.value()
        };
    }

    /**
     * @brief Create a query for filtering stocks based on the criteria
     * specified in the StockFilter struct, this factory method takes a
     * StockFilter object as input and generates an orm::Query object that can
     * be used to query the database for stocks that match the specified
     * criteria, allowing for filtering by stock IDs, instrument IDs, and
     * tickers.
     *
     * @param filter
     * @return orm::Query
     */
    orm::Query InstrumentFactory::toStockQuery(
        const finance::StockFilter& filter
    )
    {
        orm::Query query{};

        if (!filter.stockIds.empty())
            query = query.in<StockRow::idField>(filter.stockIds);

        if (!filter.instrumentIds.empty())
            query = query.in<StockRow::instrumentIdField>(filter.instrumentIds);

        if (!filter.tickers.empty())
            query = query.in<StockRow::tickerField>(filter.tickers);

        return query;
    }

}   // namespace repo

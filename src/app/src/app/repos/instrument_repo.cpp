#include "instrument_repo.hpp"

#include "app/factories/instrument_factory.hpp"
#include "config/id_types.hpp"
#include "finance/stock.hpp"
#include "orm/crud.hpp"
#include "sql_models/stock_row.hpp"

namespace app
{

    /**
     * @brief Get a list of all stock rows in the database, this is a helper
     * method that retrieves all rows from the stock table, which can then be
     * used to construct Stock objects for use in the application, ensuring that
     * the data from the database is correctly mapped to the properties of the
     * Stock objects.
     *
     * @return std::vector<StockRow>
     */
    std::vector<StockRow> InstrumentRepo::_getStockRows()
    {
        return _getCrud().get<StockRow>(_getDb());
    }

    /**
     * @brief get a list of all stock tickers in the database
     *
     * @return std::vector<std::string>
     */
    std::vector<std::string> InstrumentRepo::getTickers()
    {
        auto results = _getStockRows() |
                       std::views::transform([](const StockRow& row)
                                             { return row.ticker.value(); });

        return {results.begin(), results.end()};
    }

    /**
     * @brief get a list of all stocks in the database, this will return all
     * stocks that are not marked as deleted, and will include stocks that are
     * new or modified but not yet saved to the database.
     *
     * @return std::vector<finance::Stock>
     */
    std::vector<finance::Stock> InstrumentRepo::getStocks()
    {
        auto results =
            _getStockRows() |
            std::views::transform([](const StockRow& row)
                                  { return InstrumentFactory::toStock(row); });

        return {results.begin(), results.end()};
    }

    /**
     * @brief add a stock instrument to the database, this involves inserting a
     * new row into the instrument table and a corresponding row into the stock
     * table, ensuring that the relationships between the tables are maintained
     * correctly.
     *
     * @param stock The Stock object containing the details of the stock to be
     * added to the database
     */
    void InstrumentRepo::addStock(const finance::Stock& stock)
    {
        auto [instrumentRow, stockRow] = InstrumentFactory::fromStock(stock);

        auto result = _getCrud().insert(_getDb(), instrumentRow);

        if (!result)
        {
            throw std::runtime_error(
                "Failed to insert instrument row: " +
                result.error().getMessage()
            );
        }

        stockRow.instrumentId = InstrumentId(result.value());
        result                = _getCrud().insert(_getDb(), stockRow);

        if (!result)
        {
            throw std::runtime_error(
                "Failed to insert stock row: " + result.error().getMessage()
            );
        }
    }

    /**
     * @brief Check if a stock with the given ticker already exists in the
     * database, this is used to prevent duplicate entries and ensure data
     * integrity.
     *
     * @param ticker The ticker symbol of the stock to check for existence
     * @return true if a stock with the given ticker exists, false otherwise
     */
    bool InstrumentRepo::stockExists(const std::string& ticker)
    {
        const auto query = orm::Query{}.where(StockRow::hasTicker(ticker));

        auto result = _getCrud().get<StockRow>(_getDb(), query);

        return !result.empty();
    }

}   // namespace app
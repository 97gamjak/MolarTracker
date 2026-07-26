#include "instrument_repo.hpp"

#include "config/id_types.hpp"
#include "finance/instrument/option.hpp"
#include "finance/instrument/options.hpp"
#include "finance/instrument/stock.hpp"
#include "orm/crud.hpp"
#include "orm/query_options.hpp"
#include "repo/factories/instrument_factory.hpp"
#include "repo_errors.hpp"
#include "sql_models/option_row.hpp"
#include "sql_models/stock_row.hpp"

namespace repo
{

    /**
     * @brief Get a list of all stock rows in the database, this is a helper
     * method that retrieves all rows from the stock table, which can then be
     * used to construct Stock objects for use in the application, ensuring that
     * the data from the database is correctly mapped to the properties of the
     * Stock objects.
     *
     * @param ids The set of instrument IDs to retrieve stock rows for
     * @return std::vector<StockRow>
     */
    std::vector<StockRow> InstrumentRepo::_getStockRows(
        const IdSet<InstrumentId>& ids
    )
    {
        orm::Query query{};

        if (!ids.empty())
            query = query.in<StockRow::instrumentIdField>(ids);

        return _getCrud().get<StockRow>(_getDb(), query);
    }

    /**
     * @brief helper method to add an instrument to the database, this will
     * insert a new row into the instrument table and return the generated
     * instrument ID, which can then be used to insert rows into the specific
     * instrument type tables (e.g., stock, option) while maintaining the
     * correct relationships between the tables.
     *
     * @param instrumentRow
     * @return InstrumentId
     */
    InstrumentId InstrumentRepo::_addInstrument(
        const InstrumentRow& instrumentRow
    )
    {
        auto result = _getCrud().insert(_getDb(), instrumentRow);

        if (!result)
        {
            throw RepositoryException(
                "Failed to insert instrument row: " +
                result.error().getMessage()
            );
        }

        return InstrumentId(result.value());
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
     * @param ids The set of instrument IDs to retrieve stocks for
     * @return std::vector<finance::Stock>
     */
    std::vector<finance::Stock> InstrumentRepo::getStocks(
        const IdSet<InstrumentId>& ids
    )
    {
        auto results =
            _getStockRows(ids) |
            std::views::transform([](const StockRow& row)
                                  { return InstrumentFactory::toStock(row); });

        return {results.begin(), results.end()};
    }

    /**
     * @brief get a list of all options in the database, this will return all
     * options that are not marked as deleted, and will include options that
     * are new or modified but not yet saved to the database.
     *
     * @return finance::Options
     */
    finance::Options InstrumentRepo::getOptions()
    {
        orm::Query query{};

        return _getOptions(query);
    }

    /**
     * @brief get a list of all options in the database for the given instrument
     * IDs, this will return all options that are not marked as deleted, and
     * will include options that are new or modified but not yet saved to the
     * database.
     *
     * @param ids The set of instrument IDs to retrieve options for
     * @return finance::Options
     */
    finance::Options InstrumentRepo::getOptions(const IdSet<InstrumentId>& ids)
    {
        orm::Query query{};

        if (!ids.empty())
            query = query.in<OptionRow::instrumentIdField>(ids);

        return _getOptions(query);
    }

    /**
     * @brief get a list of all options in the database for the given query,
     * this is a helper method that retrieves option rows based on the provided
     * query, and then constructs Option objects for use in the application,
     * ensuring that the data from the database is correctly mapped to the
     * properties of the Option objects.
     *
     * @param query The ORM query to filter the option rows
     * @return finance::Options The resulting Options object containing the
     * retrieved options
     */
    finance::Options InstrumentRepo::_getOptions(const orm::Query& query)
    {
        auto join = orm::Joins{}.add(
            orm::join<
                OptionRow::underlyingInstrumentIdField,
                StockRow::instrumentIdField>()
        );

        const auto options =
            _getCrud().getJoined<OptionRow, StockRow>(_getDb(), join, query) |
            std::views::transform(
                [](const std::tuple<OptionRow, StockRow>& rows)
                {
                    return InstrumentFactory::toOption(
                        std::get<0>(rows),
                        std::get<1>(rows)
                    );
                }
            );

        return {options};
    }

    /**
     * @brief get a stock by its ticker symbol, this allows callers to retrieve
     * a specific stock from the database based on its ticker, which is a
     * common identifier for stocks and can be used to quickly access the
     * stock's details without needing to know its instrument ID.
     *
     * @param ticker The ticker symbol of the stock to retrieve
     * @return std::optional<finance::Stock> The Stock object if found, or an
     * empty optional if no stock with the given ticker exists in the database
     */
    std::optional<finance::Stock> InstrumentRepo::getStock(
        const std::string& ticker
    )
    {
        const auto query = orm::Query{}.where(StockRow::hasTicker(ticker));

        auto result = _getCrud().getUnique<StockRow>(_getDb(), query);

        if (!result)
            return std::nullopt;

        return InstrumentFactory::toStock(result.value());
    }

    /**
     * @brief add a stock instrument to the database, this involves inserting a
     * new row into the instrument table and a corresponding row into the stock
     * table, ensuring that the relationships between the tables are maintained
     * correctly.
     *
     * @param stock The Stock object containing the details of the stock to be
     * added to the database
     *
     * @return A struct containing the StockId and InstrumentId of the newly
     * added stock
     */
    finance::StockInsertionResult InstrumentRepo::addStock(
        const finance::Stock& stock
    )
    {
        auto [instrumentRow, stockRow] = InstrumentFactory::fromStock(stock);

        const auto instrumentId = _addInstrument(instrumentRow);

        stockRow.instrumentId = instrumentId;
        const auto result     = _getCrud().insert(_getDb(), stockRow);

        if (!result)
        {
            throw RepositoryException(
                "Failed to insert stock row: " + result.error().getMessage()
            );
        }

        return {
            .stockId      = StockId(result.value()),
            .instrumentId = stockRow.instrumentId.value()
        };
    }

    /**
     * @brief add an option instrument to the database, this involves inserting
     * a new row into the instrument table and a corresponding row into the
     * option table, ensuring that the relationships between the tables are
     * maintained correctly, and that the underlying stock information is also
     * added to the database if it does not already exist.
     *
     * @param option The Option object containing the details of the option to
     * be added to the database
     *
     * @return A struct containing the OptionId and InstrumentId of the newly
     * added option
     */
    finance::OptionInsertionResult InstrumentRepo::addOption(
        const finance::Option& option
    )
    {
        const auto& stock = option.getUnderlying();
        if (!stockExists(stock.getTicker()))
        {
            [[maybe_unused]] const auto stockResult = addStock(stock);
        }

        auto [instrumentRow, optionRow] = InstrumentFactory::fromOption(option);

        const auto instrumentId = _addInstrument(instrumentRow);

        optionRow.instrumentId = instrumentId;
        const auto result      = _getCrud().insert(_getDb(), optionRow);

        if (!result)
        {
            throw RepositoryException(
                "Failed to insert option row: " + result.error().getMessage()
            );
        }

        return {
            .optionId     = OptionId(result.value()),
            .instrumentId = optionRow.instrumentId.value()
        };
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

    /**
     * @brief Check if an option with the given details already exists in the
     * database, this is used to prevent duplicate entries and ensure data
     * integrity.
     *
     * @param option The Option object containing the details of the option to
     * check for existence
     * @return true if an option with the given details exists, false otherwise
     */
    bool InstrumentRepo::optionExists(const finance::Option& option)
    {
        const auto query = orm::Query{}.where(
            OptionRow::hasName(
                option.getUnderlying().getInstrumentId(),
                option.getOptionType(),
                option.getStrikePrice().getAmount(),
                option.getExpirationDate()
            )
        );

        auto result = _getCrud().get<OptionRow>(_getDb(), query);

        return !result.empty();
    }

}   // namespace repo
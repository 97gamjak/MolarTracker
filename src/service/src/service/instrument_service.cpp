#include "instrument_service.hpp"

#include "finance/instrument/option.hpp"
#include "finance/instrument/options.hpp"
#include "finance/instrument/stock.hpp"
#include "repo/i_instrument_repo.hpp"

namespace service
{
    /**
     * @brief Construct a new Instrument Service object
     *
     * @param instrumentRepo
     */
    InstrumentService::InstrumentService(
        const std::shared_ptr<repo::IInstrumentRepo>& instrumentRepo
    )
        : _instrumentRepo(instrumentRepo)
    {
    }

    /**
     * @brief get a list of all stock tickers in the database
     *
     * @return std::vector<std::string>
     */
    std::vector<std::string> InstrumentService::getTickers()
    {
        return _instrumentRepo->getTickers();
    }

    /**
     * @brief get a list of all stocks in the database
     *
     * @param filter The filter parameters to restrict the query a specific set
     * of stocks
     * @return std::vector<finance::Stock>
     */
    std::vector<finance::Stock> InstrumentService::getStocks(
        const finance::SecuritiesFilter& filter
    )
    {
        return _instrumentRepo->getStocks(filter);
    }

    /**
     * @brief get a list of all options in the database, this will return all
     * options that are not marked as deleted, and will include options that
     * are new or modified but not yet saved to the database.
     *
     * @return finance::Options
     */
    finance::Options InstrumentService::getOptions()
    {
        return _instrumentRepo->getOptions();
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
    finance::Options InstrumentService::getOptions(
        const IdSet<InstrumentId>& ids
    )
    {
        return _instrumentRepo->getOptions(ids);
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
    std::optional<finance::Stock> InstrumentService::getStock(
        const std::string& ticker
    )
    {
        return _instrumentRepo->getStock(ticker);
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
     * @return CrudResult<finance::StockInsertionResult> The result of the add
     * operation, containing the StockId and InstrumentId of the newly added
     * stock
     */
    CrudResult<finance::StockInsertionResult> InstrumentService::addStock(
        const finance::Stock& stock
    )
    {
        return _instrumentRepo->addStock(stock);
    }

    /**
     * @brief add an option instrument to the database, this involves inserting
     * a new row into the instrument table and a corresponding row into the
     * option table, ensuring that the relationships between the tables are
     * maintained correctly.
     *
     * @param option The Option object containing the details of the option to
     * be added to the database
     *
     * @return CrudResult<finance::OptionInsertionResult> The result of the add
     * operation, containing the OptionId and InstrumentId of the newly added
     * option
     */
    CrudResult<finance::OptionInsertionResult> InstrumentService::addOption(
        const finance::Option& option
    )
    {
        return _instrumentRepo->addOption(option);
    }

    /**
     * @brief Check if a stock with the given ticker already exists in the
     * database, this is used to prevent duplicate entries and ensure data
     * integrity.
     *
     * @param ticker The ticker symbol of the stock to check for existence
     * @return true if a stock with the given ticker exists, false otherwise
     */
    bool InstrumentService::stockExists(const std::string& ticker)
    {
        return _instrumentRepo->stockExists(ticker);
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
    bool InstrumentService::optionExists(const finance::Option& option)
    {
        return _instrumentRepo->optionExists(option);
    }

}   // namespace service

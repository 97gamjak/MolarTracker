#include "instrument_service.hpp"

#include "finance/instrument/option.hpp"
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
     * @param ids The set of instrument IDs to retrieve stocks for
     * @return std::vector<finance::Stock>
     */
    std::vector<finance::Stock> InstrumentService::getStocks(
        const IdSet<InstrumentId>& ids
    )
    {
        return _instrumentRepo->getStocks(ids);
    }

    std::vector<finance::Option> InstrumentService::getOptions()
    {
        return _instrumentRepo->getOptions();
    }

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
     * @return A struct containing the StockId and InstrumentId of the newly
     * added stock
     */
    finance::StockInsertionResult InstrumentService::addStock(
        const finance::Stock& stock
    )
    {
        return _instrumentRepo->addStock(stock);
    }

    finance::OptionInsertionResult InstrumentService::addOption(
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

    bool InstrumentService::optionExists(const finance::Option& option)
    {
        return _instrumentRepo->optionExists(option);
    }

}   // namespace service

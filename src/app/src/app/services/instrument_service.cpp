#include "instrument_service.hpp"

#include "app/repos_api/i_instrument_repo.hpp"
#include "finance/stock.hpp"

namespace app
{
    /**
     * @brief Construct a new Instrument Service object
     *
     * @param instrumentRepo
     */
    InstrumentService::InstrumentService(
        const std::shared_ptr<IInstrumentRepo>& instrumentRepo
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
     * @return std::vector<finance::Stock>
     */
    std::vector<finance::Stock> InstrumentService::getStocks()
    {
        return _instrumentRepo->getStocks();
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
     * @return A pair containing the StockId and InstrumentId of the newly
     * added stock
     */
    std::pair<StockId, InstrumentId> InstrumentService::addStock(
        const finance::Stock& stock
    )
    {
        return _instrumentRepo->addStock(stock);
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

}   // namespace app

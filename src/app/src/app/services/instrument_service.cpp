#include "instrument_service.hpp"

#include "app/repos_api/i_instrument_repo.hpp"

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
    [[nodiscard]] std::vector<std::string> InstrumentService::getTickers()
    {
        return _instrumentRepo->getTickers();
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
    void InstrumentService::addStock(const finance::Stock& stock)
    {
        _instrumentRepo->addStock(stock);
    }

}   // namespace app

#ifndef __APP__INCLUDE__APP__SERVICES_API__I_INSTRUMENT_SERVICE_HPP__
#define __APP__INCLUDE__APP__SERVICES_API__I_INSTRUMENT_SERVICE_HPP__

#include <string>
#include <vector>

namespace finance
{
    class Stock;
}   // namespace finance

namespace app
{
    /**
     * @brief Interface for the Instrument service, this defines the methods
     * that the InstrumentService class must implement for managing instruments
     * in the application.
     */
    class IInstrumentService
    {
       public:
        virtual ~IInstrumentService() = default;

        /**
         * @brief Get a list of all stock tickers in the database
         *
         * @return std::vector<std::string>
         */
        [[nodiscard]]
        virtual std::vector<std::string> getTickers() = 0;

        /**
         * @brief get a list of all stocks in the database
         *
         * @return std::vector<finance::Stock>
         */
        virtual std::vector<finance::Stock> getStocks() = 0;

        /**
         * @brief Add a stock instrument to the database, this involves
         * inserting a new row into the instrument table and a corresponding row
         * into the stock table, ensuring that the relationships between the
         * tables are maintained correctly.
         *
         * @param stock The Stock object containing the details of the stock to
         * be added to the database
         */
        virtual void addStock(const finance::Stock& stock) = 0;

        /**
         * @brief Check if a stock with the given ticker already exists in the
         * database, this is used to prevent duplicate entries and ensure data
         * integrity.
         *
         * @param ticker The ticker symbol of the stock to check for existence
         * @return true if a stock with the given ticker exists, false otherwise
         */
        [[nodiscard]]
        virtual bool stockExists(const std::string& ticker) = 0;
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__SERVICES_API__I_INSTRUMENT_SERVICE_HPP__
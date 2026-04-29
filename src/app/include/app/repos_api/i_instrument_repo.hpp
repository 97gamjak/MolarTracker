#ifndef __APP__INCLUDE__APP__REPOS_API__I_INSTRUMENT_REPO_HPP__
#define __APP__INCLUDE__APP__REPOS_API__I_INSTRUMENT_REPO_HPP__

#include <string>
#include <vector>

namespace finance
{
    class Stock;
}   // namespace finance

namespace app
{
    /**
     * @brief Interface for the Instrument repository, this defines the methods
     * that the InstrumentRepo class must implement for managing instruments in
     * the database.
     */
    class IInstrumentRepo
    {
       public:
        virtual ~IInstrumentRepo() = default;

        /**
         * @brief Get a list of all stock tickers in the database
         *
         * @return std::vector<std::string>
         */
        virtual std::vector<std::string> getTickers() = 0;

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
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__REPOS_API__I_INSTRUMENT_REPO_HPP__
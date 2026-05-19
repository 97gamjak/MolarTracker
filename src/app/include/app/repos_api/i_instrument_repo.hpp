#ifndef __APP__INCLUDE__APP__REPOS_API__I_INSTRUMENT_REPO_HPP__
#define __APP__INCLUDE__APP__REPOS_API__I_INSTRUMENT_REPO_HPP__

#include <string>
#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    class Stock;
    struct StockInsertionResult;
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
        [[nodiscard]]
        virtual std::vector<std::string> getTickers() = 0;

        /**
         * @brief get a list of all stocks in the database
         *
         * @param ids The set of instrument IDs to retrieve stocks for
         * @return std::vector<finance::Stock>
         */
        [[nodiscard]]
        virtual std::vector<finance::Stock> getStocks(
            const idSet<InstrumentId>& ids
        ) = 0;

        /**
         * @brief Get a stock by its ticker symbol
         *
         * @param ticker The ticker symbol of the stock to retrieve
         * @return std::optional<finance::Stock>
         */
        [[nodiscard]]
        virtual std::optional<finance::Stock> getStock(
            const std::string& ticker
        ) = 0;

        /**
         * @brief Add a stock instrument to the database, this involves
         * inserting a new row into the instrument table and a corresponding row
         * into the stock table, ensuring that the relationships between the
         * tables are maintained correctly.
         *
         * @param stock The Stock object containing the details of the stock to
         * be added to the database
         *
         * @return A struct containing the StockId and InstrumentId of the newly
         * added stock
         */
        [[nodiscard]]
        virtual finance::StockInsertionResult addStock(
            const finance::Stock& stock
        ) = 0;

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

#endif   // __APP__INCLUDE__APP__REPOS_API__I_INSTRUMENT_REPO_HPP__
#ifndef __SERVICE__INCLUDE__SERVICE__I_INSTRUMENT_SERVICE_HPP__
#define __SERVICE__INCLUDE__SERVICE__I_INSTRUMENT_SERVICE_HPP__

#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "utils/container/set.hpp"

namespace finance
{
    class Stock;                    // forward declaration
    struct StockInsertionResult;    // forward declaration
    class Option;                   // forward declaration
    struct OptionInsertionResult;   // forward declaration

}   // namespace finance

namespace service
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
         * @param ids The set of instrument IDs to retrieve stocks for
         * @return std::vector<finance::Stock>
         */
        [[nodiscard]]
        virtual std::vector<finance::Stock> getStocks(
            const IdSet<InstrumentId>& ids
        ) = 0;

        /**
         * @brief get a list of all options in the database, this will return
         * all options that are not marked as deleted, and will include options
         * that are new or modified but not yet saved to the database.
         *
         * @return std::vector<finance::Option>
         */
        [[nodiscard]]
        virtual std::vector<finance::Option> getOptions() = 0;

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
         * @brief Add an option instrument to the database, this involves
         * inserting a new row into the instrument table and a corresponding row
         * into the option table, ensuring that the relationships between the
         * tables are maintained correctly.
         *
         * @param option The Option object containing the details of the option
         * to be added to the database
         *
         * @return A struct containing the OptionId and InstrumentId of the
         * newly added option
         */
        [[nodiscard]]
        virtual finance::OptionInsertionResult addOption(
            const finance::Option& option
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

        /**
         * @brief Check if an option with the given details already exists in
         * the database, this is used to prevent duplicate entries and ensure
         * data integrity.
         *
         * @param option The Option object containing the details of the option
         * to check for existence
         * @return true if an option with the given details exists, false
         * otherwise
         */
        [[nodiscard]]
        virtual bool optionExists(const finance::Option& option) = 0;
    };
}   // namespace service

#endif   // __SERVICE__INCLUDE__SERVICE__I_INSTRUMENT_SERVICE_HPP__
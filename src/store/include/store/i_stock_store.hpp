#ifndef __STORE__INCLUDE__STORE__I_STOCK_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_STOCK_STORE_HPP__

#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "config/signal_tags.hpp"

class Connection;   // Forward declaration

namespace finance
{
    class Stock;   // forward declaration
}   // namespace finance

namespace store
{
    /**
     * @brief Result of stock store operations
     *
     */
    enum class StockStoreResult : std::uint8_t
    {
        Ok,
        Error,
        StockAlreadyExists,
        StockNotFound,
    };

    /**
     * @brief Store for managing stocks
     *
     */
    class IStockStore
    {
       public:
        virtual ~IStockStore() = default;

        /**
         * @brief Add a stock to the store, this will check if a stock with the
         * same ticker already exists in the store or in the database, and if
         * not, it will add the stock to the store and the database.
         *
         * @param stock The Stock object to be added to the store
         * @return StockStoreResult indicating the result of the operation
         */
        [[nodiscard]]
        virtual StockStoreResult addStock(finance::Stock stock) = 0;

        [[nodiscard]]
        virtual std::optional<finance::Stock> getStock(
            InstrumentId id
        ) const = 0;

        /**
         * @brief Get a list of stocks by their instrument IDs
         *
         * @param ids The set of instrument IDs to retrieve stocks for
         * @return std::vector<finance::Stock>
         */
        [[nodiscard]]
        virtual std::vector<finance::Stock> getStocks(
            const idSet<InstrumentId>& ids
        ) const = 0;

        /**
         * @brief Get a list of all stocks in the store
         *
         * @return std::vector<finance::Stock>
         */
        [[nodiscard]]
        virtual std::vector<finance::Stock> getStocks() const = 0;

        /**
         * @brief Get all stock tickers in the store
         *
         * @return std::vector<std::string>
         */
        [[nodiscard]]
        virtual std::vector<std::string> getAllTickers() const = 0;

        /**
         * @brief Get a mapping of stock tickers to their instrument IDs
         *
         * @return std::unordered_map<std::string, InstrumentId>
         */
        [[nodiscard]]
        virtual instrumentMap<std::string> getInstrumentIdToNameMap() const = 0;

        /**
         * @brief Get the instrument ID for a given stock ticker
         *
         * @param ticker The stock ticker
         * @return std::optional<InstrumentId>
         */
        [[nodiscard]]
        virtual std::optional<InstrumentId> getInstrumentId(
            const std::string& ticker
        ) const = 0;

        /**
         * @brief commit changes to the store, this will save any new or
         * modified stocks to the database, and will also handle any necessary
         * cleanup or state updates in the store after committing changes.
         *
         */
        virtual void commit() = 0;

        /**
         * @brief Get the mapping of old instrument IDs to new instrument IDs
         *
         * @return const unorderedIdMap<InstrumentId, InstrumentId>&
         */
        [[nodiscard]]
        virtual const unorderedIdMap<
            InstrumentId,
            InstrumentId>& getInstrumentIdMap() const = 0;

        /**
         * @brief Subscribe to changes in the stock store, this allows observers
         * to be notified when the stock store changes, such as when a new stock
         * is added, or when a stock is updated or deleted. This is useful for
         * keeping the UI in sync with the underlying data model, and for
         * allowing other parts of the application to react to changes in the
         * stock store.
         *
         * @param func The callback function to call when the stock store
         * changes, the function will receive a StockId as a parameter, which
         * indicates which stock was changed, and the user pointer can be used
         * to pass additional data to the callback function, the returned
         * Connection object can be used to unsubscribe from changes
         * @param subscriber A pointer to the subscriber object, this is used to
         * identify the subscriber when subscribing and unsubscribing from
         * changes, it can be any pointer that the subscriber chooses, and will
         * be passed to the callback function when it is called
         *
         * @return Connection
         */
        [[nodiscard]]
        virtual Connection subscribeToStoreChange(
            StoreChanged<StockId>::func func,
            void*                       subscriber
        ) = 0;
    };
}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_STOCK_STORE_HPP__
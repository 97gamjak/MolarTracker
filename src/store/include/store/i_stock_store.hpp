#ifndef __STORE__INCLUDE__STORE__I_STOCK_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_STOCK_STORE_HPP__

#include <optional>
#include <string>

#include "config/id_types.hpp"
#include "finance/instrument/stocks.hpp"
#include "subscriptions.hpp"
#include "utils/container/id_id_map.hpp"

class Connection;   // Forward declaration

namespace finance
{
    class Stock;          // forward declaration
    struct StockFilter;   // forward declaration
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
     * @brief Interface for reading stocks from a store
     *
     */
    class IStockStoreReader
    {
       protected:
        /**
         * @brief Struct representing a callback for when a commit occurs in the
         * stock store, this can be used to emit an event when a commit occurs,
         * allowing other parts of the application to react to the commit event.
         *
         */
        struct OnCommit
        {
            /// Type alias for the commit callback function
            using func = std::function<void(
                const IdIdMap<StockId>&      stockIdMap,
                const IdIdMap<InstrumentId>& instrumentIdMap
            )>;
        };

       public:
        virtual ~IStockStoreReader() = default;

        /**
         * @brief Get the Stock based on the given stock ID
         *
         * @param id
         * @return std::optional<finance::Stock>
         */
        [[nodiscard]]
        virtual std::optional<finance::Stock> getStock(StockId id) const = 0;

        /**
         * @brief Get the Stock based on the given instrument ID
         *
         * @param id
         * @return std::optional<finance::Stock>
         */
        [[nodiscard]]
        virtual std::optional<finance::Stock> getStock(
            InstrumentId id
        ) const = 0;

        /**
         * @brief Get the Stock based on the given ticker
         *
         * @param ticker
         * @return std::optional<finance::Stock>
         */
        [[nodiscard]]
        virtual std::optional<finance::Stock> getStock(
            const std::string& ticker
        ) const = 0;

        /**
         * @brief Get a list of all stocks in the store
         *
         * @param filter The filter to apply when retrieving stocks
         * @return finance::Stocks
         */
        [[nodiscard]]
        virtual finance::Stocks getStocks(
            const finance::StockFilter& filter
        ) const = 0;

        /**
         * @brief Subscribe to commit events
         *
         * @param func The callback function to be called when a commit occurs
         * @param subscriber The subscriber object that will receive the event
         * @return Connection The connection object for managing the
         * subscription
         */
        [[nodiscard]]
        virtual Connection subscribeToCommit(
            const OnCommit::func& func,
            void*                 subscriber
        ) = 0;

        /// @cond DOXYGEN_IGNORE
        SUBSCRIBE_VIRTUAL(finance::Stock, StockId)
        /// @endcond
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
    };
}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_STOCK_STORE_HPP__
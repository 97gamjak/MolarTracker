#ifndef __APP__INCLUDE__APP__STORE__STOCK_STORE_HPP__
#define __APP__INCLUDE__APP__STORE__STOCK_STORE_HPP__

#include <memory>

#include "app/services_api/i_instrument_service.hpp"
#include "base/base_store.hpp"
#include "finance/stock.hpp"

namespace app
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
    class StockStore : public BaseStore<finance::Stock, StockId>
    {
       private:
        /// The type of the Instrument service pointer
        using InstrumentServicePtr = std::shared_ptr<IInstrumentService>;
        /// The Instrument service
        InstrumentServicePtr _instrumentService;

       public:
        explicit StockStore(InstrumentServicePtr instrumentService);

        StockStoreResult addStock(finance::Stock stock);

        std::vector<finance::Stock> getStocks() const;

        std::vector<std::string> getAllTickers() const;

        void commit();
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__STOCK_STORE_HPP__
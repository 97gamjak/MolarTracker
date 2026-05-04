#ifndef __APP__INCLUDE__APP__STORE__STOCK_STORE_HPP__
#define __APP__INCLUDE__APP__STORE__STOCK_STORE_HPP__

#include <memory>
#include <unordered_map>

#include "app/services_api/i_instrument_service.hpp"
#include "base/base_store.hpp"
#include "config/id_types.hpp"
#include "config/signal_tags.hpp"
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
       public:
        using InstrumentIdMap =
            std::unordered_map<InstrumentId, InstrumentId, InstrumentId::Hash>;

       private:
        /// The type of the Instrument service pointer
        using InstrumentServicePtr = std::shared_ptr<IInstrumentService>;

        /// The Instrument service
        InstrumentServicePtr _instrumentService;

        InstrumentIdSeq& _instrumentIdSeq;

        Observable<OnIdRemap<InstrumentId>> _onInstrumentIdRemap;

       public:
        explicit StockStore(
            InstrumentServicePtr instrumentService,
            InstrumentIdSeq&     instrumentIdSeq
        );

        ~StockStore() override                   = default;
        StockStore(const StockStore&)            = delete;
        StockStore& operator=(const StockStore&) = delete;
        StockStore(StockStore&&)                 = delete;
        StockStore& operator=(StockStore&&)      = delete;

        [[nodiscard]]
        StockStoreResult addStock(finance::Stock stock);

        [[nodiscard]]
        std::vector<finance::Stock> getStocks() const;

        [[nodiscard]]
        std::vector<std::string> getAllTickers() const;

        [[nodiscard]]
        std::unordered_map<std::string, InstrumentId> getTickerMap() const;

        [[nodiscard]]
        std::unordered_map<
            InstrumentId,
            std::string,
            typename InstrumentId::Hash> getInstrumentIdToNameMap() const;

        [[nodiscard]]
        bool stockExists(const std::string& ticker, bool checkDeleted) const;

        [[nodiscard]]
        std::optional<InstrumentId> getInstrumentId(
            const std::string& ticker
        ) const;

        void commit();

        Connection subscribeToInstrumentIdRemap(
            OnIdRemap<InstrumentId>::func func,
            void*                         userData
        );
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__STOCK_STORE_HPP__
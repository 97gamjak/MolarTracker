#ifndef __CONTROLLER__SRC__CONTROLLER__POSITION_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__POSITION_CONTROLLER_HPP__

#include <qtmetamacros.h>

#include <QFutureWatcher>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "cache/account_cache.hpp"
#include "finance/price_quote.hpp"

class QTimer;   // Forward declaration

namespace store
{
    class IPositionStore;      // Forward declaration
    class ITransactionStore;   // Forward declaration
}   // namespace store

namespace cache
{
    class StockCache;     // Forward declaration
    class AccountCache;   // Forward declaration
}   // namespace cache

namespace finance
{
    class PriceCache;     // Forward declaration
    class Transactions;   // Forward declaration
}   // namespace finance

class Connections;   // Forward declaration

namespace controller
{
    /**
     * @brief Controller for managing positions and their associated data.
     *
     */
    class PositionController : public QObject
    {
        Q_OBJECT

       private:
        /// Watcher for asynchronous price fetching operations
        QFutureWatcher<std::unordered_map<std::string, finance::PriceQuote>>
            _priceWatcher;
        /// Timer for periodically fetching price updates
        QTimer* _pollTimer;
        /// Cache for storing the latest price quotes for the tracked tickers
        std::shared_ptr<finance::PriceCache> _priceCache;

        /// Reference to the position store for retrieving position data
        std::shared_ptr<store::IPositionStore> _positionStore;
        /// Reference to the transaction store for retrieving transaction data
        std::shared_ptr<store::ITransactionStore> _transactionStore;
        /// Reference to the stock cache for retrieving stock data (e.g.
        /// tickers)
        std::shared_ptr<cache::StockCache> _stockCache;
        /// Reference to the account cache for retrieving account data (e.g.
        /// account IDs)
        std::shared_ptr<cache::AccountCache> _accountCache;

        /// The expected number of symbols to be fetched in the price update,
        /// used for gating updates to the price cache
        std::size_t _expectedSymbolCount;

        /// Connections object for managing signal-slot connections and ensuring
        /// they are properly cleaned up
        std::unique_ptr<Connections> _connections;

        /// Set of tickers that are currently being tracked for price updates
        std::unordered_set<std::string> _tickers;

       public:
        PositionController(
            const std::shared_ptr<store::IPositionStore>&    positionStore,
            const std::shared_ptr<store::ITransactionStore>& transactionStore,
            const std::shared_ptr<cache::StockCache>&        stockCache,
            const std::shared_ptr<cache::AccountCache>&      accountCache,
            const std::shared_ptr<finance::PriceCache>&      priceCache
        );
        ~PositionController() override;

       private:
        void _fetchPrices();
        void _onPricesFetched();
        void _collectTickers(const finance::Transactions& transactions);
        void _initTickers();
    };
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__POSITION_CONTROLLER_HPP__

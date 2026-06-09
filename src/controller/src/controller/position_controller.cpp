#include "position_controller.hpp"

#include <QFuture>
#include <QTimer>
#include <QtConcurrent>
#include <memory>

#include "connections/connection.hpp"
#include "finance/price_cache.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "store/i_position_store.hpp"
#include "store/i_stock_store.hpp"
#include "store/i_transaction_store.hpp"

namespace controller
{
    PositionController::PositionController(
        const std::shared_ptr<store::IPositionStore>&    positionStore,
        const std::shared_ptr<store::ITransactionStore>& transactionStore,
        const std::shared_ptr<store::IStockStore>&       stockStore,
        const std::shared_ptr<finance::PriceCache>&      priceCache
    )
        : _pollTimer(new QTimer()),
          _priceCache(priceCache),
          _positionStore(positionStore),
          _transactionStore(transactionStore),
          _stockStore(stockStore),
          _expectedSymbolCount(0),
          _connections(std::make_unique<Connections>())
    {
        connect(
            &_priceWatcher,
            &QFutureWatcher<
                std::unordered_map<std::string, finance::PriceQuote>>::finished,
            this,
            &PositionController::_onPricesFetched
        );

        connect(
            _pollTimer,
            &QTimer::timeout,
            this,
            &PositionController::_fetchPrices
        );

        _initTickers();

        _connections->add(_transactionStore->subscribeToTransactionAdded(
            [this](const finance::Transactions& transactions)
            {
                _collectTickers(transactions);
                _fetchPrices();
            },
            this
        ));

        _pollTimer->setInterval(60'000);

        _fetchPrices();
        _pollTimer->start();
    }

    PositionController::~PositionController() = default;

    void PositionController::_fetchPrices()
    {
        if (_priceWatcher.isRunning())
            return;   // don't stack concurrent fetches

        if (_tickers.empty())
            return;

        _expectedSymbolCount = _tickers.size();

        _priceWatcher.setFuture(
            QtConcurrent::run(
                [tickers = _tickers]()
                { return finance::PriceFeedService::fetchBatch(tickers); }
            )
        );
    }

    void PositionController::_onPricesFetched()
    {
        const auto result = _priceWatcher.result();
        // Gate: only update if we got back the full symbol set
        if (result.size() == _expectedSymbolCount)
            _priceCache->update(result);

        // Notify table model — adjust column indices to your price/P&L range
        // emit _tableModel->dataChanged(...);
    }

    void PositionController::_collectTickers(
        const finance::Transactions& transactions
    )
    {
        const auto instruments   = transactions.securities();
        const auto instrumentIds = instruments.getBaseInstrumentIds();

        const auto tickers = _stockStore->getStocks(instrumentIds).getTickers();
        for (const auto& ticker : tickers)
            _tickers.insert(ticker);
    }

    void PositionController::_initTickers()
    {
        const auto ids = _positionStore->getOpenPositions().getIds();

        finance::TransactionFilter filter;
        filter.setPositionIds(ids);

        const auto transactions = _transactionStore->getTransactions(filter);
        _collectTickers(transactions);
    }

}   // namespace controller

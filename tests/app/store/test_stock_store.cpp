#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "finance/instrument/stock.hpp"
#include "mock_services.hpp"
#include "store/stock_store.hpp"
#include "utils/finance.hpp"

namespace
{

    class StockStoreTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::shared_ptr<tests::MockInstrumentService> _mockService;
        InstrumentIdSeq                               _idSeq;
        std::unique_ptr<store::StockStore>            _store;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        StockStoreTest()
            : _mockService{std::make_shared<tests::MockInstrumentService>()},
              _store{std::make_unique<store::StockStore>(_mockService, _idSeq)}
        {
        }

        [[nodiscard]] static finance::Stock makeStock(
            const std::string& ticker     = "AAPL",
            Currency           currency   = Currency::USD,
            AssetClass         assetClass = AssetClass::Stock
        )
        {
            return finance::Stock{
                ticker,
                currency,
                "Apple Inc.",
                "Apple Incorporated",
                "NASDAQ",
                "Consumer Electronics",
                "Technology",
                assetClass
            };
        }
    };

}   // namespace

TEST_F(StockStoreTest, AddStockSuccessReturnsOk)
{
    const auto result = _store->addStock(makeStock());

    EXPECT_EQ(result, store::StockStoreResult::Ok);
}

TEST_F(StockStoreTest, AddStockAlreadyExistsInServiceReturnsStockAlreadyExists)
{
    _mockService->stocksInDb.insert("AAPL");

    const auto result = _store->addStock(makeStock("AAPL"));

    if (!_store->isFullCache())
        EXPECT_EQ(result, store::StockStoreResult::StockAlreadyExists);
    else
        EXPECT_EQ(result, store::StockStoreResult::Ok);
}

TEST_F(StockStoreTest, GetAllTickersEmptyInitially)
{
    const auto tickers = _store->getAllTickers();

    EXPECT_TRUE(tickers.empty());
}

TEST_F(StockStoreTest, GetAllTickersReturnsAddedStock)
{
    static_cast<void>(_store->addStock(makeStock("AAPL")));

    const auto tickers = _store->getAllTickers();

    ASSERT_EQ(tickers.size(), 1U);
    EXPECT_EQ(tickers.front(), "AAPL");
}

TEST_F(StockStoreTest, StockExistsFalseForUnknown)
{
    EXPECT_FALSE(_store->stockExists("UNKNOWN", false));
}

TEST_F(StockStoreTest, StockExistsTrueForAddedStock)
{
    static_cast<void>(_store->addStock(makeStock("AAPL")));

    EXPECT_TRUE(_store->stockExists("AAPL", false));
}

TEST_F(StockStoreTest, GetInstrumentIdNulloptForUnknown)
{
    const auto id = _store->getInstrumentId("UNKNOWN");

    EXPECT_FALSE(id.has_value());
}

TEST_F(StockStoreTest, GetInstrumentIdReturnsIdForAddedStock)
{
    static_cast<void>(_store->addStock(makeStock("AAPL")));

    const auto id = _store->getInstrumentId("AAPL");

    EXPECT_TRUE(id.has_value());
}

TEST_F(StockStoreTest, CommitNewStockCallsService)
{
    static_cast<void>(_store->addStock(makeStock("AAPL")));

    _store->commit();

    EXPECT_EQ(_mockService->addStockCallCount, 1);
}

TEST_F(StockStoreTest, IsDirtyFalseInitially)
{
    EXPECT_FALSE(_store->isDirty());
}

TEST_F(StockStoreTest, IsDirtyTrueAfterAddStock)
{
    static_cast<void>(_store->addStock(makeStock("AAPL")));

    EXPECT_TRUE(_store->isDirty());
}

#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "app/store/stock_store.hpp"
#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "config/strong_id.hpp"
#include "finance/instrument/stock.hpp"
#include "mock_services.hpp"

namespace
{

    class StockStoreTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::shared_ptr<tests::MockInstrumentService> _mockService;
        InstrumentIdSeq                               _idSeq;
        std::unique_ptr<app::StockStore>              _store;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        StockStoreTest()
            : _mockService{
                  std::make_shared<tests::MockInstrumentService>()
              },
              _store{
                  std::make_unique<app::StockStore>(_mockService, _idSeq)
              }
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

TEST_F(StockStoreTest, AddStock_Success_ReturnsOk)
{
    const auto result = _store->addStock(makeStock());

    EXPECT_EQ(result, app::StockStoreResult::Ok);
}

TEST_F(
    StockStoreTest,
    AddStock_AlreadyExistsInService_ReturnsStockAlreadyExists
)
{
    _mockService->stocksInDb.insert("AAPL");

    const auto result = _store->addStock(makeStock("AAPL"));

    EXPECT_EQ(result, app::StockStoreResult::StockAlreadyExists);
}

TEST_F(StockStoreTest, GetAllTickers_EmptyInitially)
{
    const auto tickers = _store->getAllTickers();

    EXPECT_TRUE(tickers.empty());
}

TEST_F(StockStoreTest, GetAllTickers_ReturnsAddedStock)
{
    static_cast<void>(_store->addStock(makeStock("AAPL")));

    const auto tickers = _store->getAllTickers();

    ASSERT_EQ(tickers.size(), 1u);
    EXPECT_EQ(tickers[0], "AAPL");
}

TEST_F(StockStoreTest, StockExists_FalseForUnknown)
{
    EXPECT_FALSE(_store->stockExists("UNKNOWN", false));
}

TEST_F(StockStoreTest, StockExists_TrueForAddedStock)
{
    static_cast<void>(_store->addStock(makeStock("AAPL")));

    EXPECT_TRUE(_store->stockExists("AAPL", false));
}

TEST_F(StockStoreTest, GetInstrumentId_NulloptForUnknown)
{
    const auto id = _store->getInstrumentId("UNKNOWN");

    EXPECT_FALSE(id.has_value());
}

TEST_F(StockStoreTest, GetInstrumentId_ReturnsIdForAddedStock)
{
    static_cast<void>(_store->addStock(makeStock("AAPL")));

    const auto id = _store->getInstrumentId("AAPL");

    EXPECT_TRUE(id.has_value());
}

TEST_F(StockStoreTest, Commit_NewStock_CallsService)
{
    static_cast<void>(_store->addStock(makeStock("AAPL")));

    _store->commit();

    EXPECT_EQ(_mockService->addStockCallCount, 1);
}

TEST_F(StockStoreTest, IsDirty_FalseInitially)
{
    EXPECT_FALSE(_store->isDirty());
}

TEST_F(StockStoreTest, IsDirty_TrueAfterAddStock)
{
    static_cast<void>(_store->addStock(makeStock("AAPL")));

    EXPECT_TRUE(_store->isDirty());
}

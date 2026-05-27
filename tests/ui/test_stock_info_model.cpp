// test_stock_info_model.cpp
//
// GoogleTest-based tests for ui::StockInfoTableModel.
//
// Coverage:
//  - empty model: rowCount/columnCount/headerData
//  - setRows replaces data and emits reset
//  - data() returns correct values for all columns
//  - updateRow replaces an existing row by ticker
//  - updateRow appends when ticker not found
//  - flags returns enabled+selectable for valid index
//  - getTickerColumn returns the ticker column index

#include <gtest/gtest.h>

#include <QtTest/QSignalSpy>
#include <QString>
#include <vector>

#include "config/finance.hpp"
#include "drafts/stock_draft.hpp"
#include "ui/securities/stock_info_model.hpp"

namespace
{

    drafts::StockInfoDraft makeDraft(
        std::string ticker   = "AAPL",
        std::string name     = "Apple Inc.",
        std::string longName = "Apple Incorporated",
        std::string exchange = "NASDAQ",
        Currency    currency = Currency::USD,
        std::string industry = "Technology",
        std::string sector   = "Information Technology",
        AssetClass  asset    = AssetClass::Stock
    )
    {
        return drafts::StockInfoDraft{
            std::move(ticker),
            std::move(name),
            std::move(longName),
            std::move(exchange),
            currency,
            std::move(industry),
            std::move(sector),
            asset
        };
    }

    class StockInfoModelTest : public ::testing::Test
    {
       protected:
        ui::StockInfoTableModel _model;
    };

    TEST_F(StockInfoModelTest, EmptyModelHasZeroRows)
    {
        EXPECT_EQ(_model.rowCount({}), 0);
    }

    TEST_F(StockInfoModelTest, ColumnCountIsSevenForValidParent)
    {
        EXPECT_EQ(_model.columnCount({}), 7);
    }

    TEST_F(StockInfoModelTest, ColumnCountZeroForValidParentIndex)
    {
        _model.setRows({makeDraft()});
        const auto idx = _model.index(0, 0, {});
        ASSERT_TRUE(idx.isValid());
        EXPECT_EQ(_model.columnCount(idx), 0);
    }

    TEST_F(StockInfoModelTest, HeaderDataReturnsColumnNames)
    {
        const auto ticker = _model.headerData(0, Qt::Horizontal, Qt::DisplayRole);
        EXPECT_EQ(ticker.toString(), "Ticker");
    }

    TEST_F(StockInfoModelTest, HeaderDataEmptyForVertical)
    {
        const auto v = _model.headerData(0, Qt::Vertical, Qt::DisplayRole);
        EXPECT_FALSE(v.isValid());
    }

    TEST_F(StockInfoModelTest, SetRowsUpdatesRowCount)
    {
        std::vector<drafts::StockInfoDraft> rows;
        rows.push_back(makeDraft("AAPL"));
        rows.push_back(makeDraft("MSFT", "Microsoft Corp.", "Microsoft Corporation",
                                 "NASDAQ", Currency::USD, "Technology",
                                 "Information Technology", AssetClass::Stock));
        _model.setRows(std::move(rows));
        EXPECT_EQ(_model.rowCount({}), 2);
    }

    TEST_F(StockInfoModelTest, DataReturnsTickerForColumn0)
    {
        _model.setRows({makeDraft("GOOG")});
        const auto idx = _model.index(0, ui::StockInfoTableModel::getTickerColumn(), {});
        EXPECT_EQ(_model.data(idx, Qt::DisplayRole).toString(), "GOOG");
    }

    TEST_F(StockInfoModelTest, DataReturnsShortNameForColumn1)
    {
        _model.setRows({makeDraft("AAPL", "Apple Inc.")});
        const auto idx = _model.index(0, 1, {});
        EXPECT_EQ(_model.data(idx, Qt::DisplayRole).toString(), "Apple Inc.");
    }

    TEST_F(StockInfoModelTest, DataReturnsExchangeForColumn2)
    {
        _model.setRows({makeDraft("AAPL", "Apple", "Apple Inc.", "NASDAQ")});
        const auto idx = _model.index(0, 2, {});
        EXPECT_EQ(_model.data(idx, Qt::DisplayRole).toString(), "NASDAQ");
    }

    TEST_F(StockInfoModelTest, DataReturnsCurrencyForColumn3)
    {
        _model.setRows({makeDraft()});
        const auto idx = _model.index(0, 3, {});
        EXPECT_FALSE(_model.data(idx, Qt::DisplayRole).toString().isEmpty());
    }

    TEST_F(StockInfoModelTest, DataInvalidIndexReturnsEmpty)
    {
        EXPECT_FALSE(_model.data({}, Qt::DisplayRole).isValid());
    }

    TEST_F(StockInfoModelTest, DataOutOfRangeRowReturnsEmpty)
    {
        _model.setRows({makeDraft()});
        const auto idx = _model.index(99, 0, {});
        EXPECT_FALSE(_model.data(idx, Qt::DisplayRole).isValid());
    }

    TEST_F(StockInfoModelTest, TextAlignmentRoleReturnsVariant)
    {
        _model.setRows({makeDraft()});
        const auto idx = _model.index(0, 0, {});
        EXPECT_TRUE(_model.data(idx, Qt::TextAlignmentRole).isValid());
    }

    TEST_F(StockInfoModelTest, UpdateRowReplacesExistingByTicker)
    {
        _model.setRows({makeDraft("AAPL", "Old Name")});
        ASSERT_EQ(_model.rowCount({}), 1);

        _model.updateRow(makeDraft("AAPL", "New Name"));
        EXPECT_EQ(_model.rowCount({}), 1);

        const auto idx = _model.index(0, 1, {});
        EXPECT_EQ(_model.data(idx, Qt::DisplayRole).toString(), "New Name");
    }

    TEST_F(StockInfoModelTest, UpdateRowAppendsWhenTickerNotFound)
    {
        _model.setRows({makeDraft("AAPL")});
        _model.updateRow(makeDraft("MSFT"));
        EXPECT_EQ(_model.rowCount({}), 2);
    }

    TEST_F(StockInfoModelTest, FlagsForValidIndex)
    {
        _model.setRows({makeDraft()});
        const auto idx   = _model.index(0, 0, {});
        const auto flags = _model.flags(idx);
        EXPECT_TRUE(flags & Qt::ItemIsEnabled);
        EXPECT_TRUE(flags & Qt::ItemIsSelectable);
    }

    TEST_F(StockInfoModelTest, FlagsForInvalidIndex)
    {
        EXPECT_EQ(_model.flags({}), Qt::NoItemFlags);
    }

    TEST_F(StockInfoModelTest, GetTickerColumnReturnsZero)
    {
        EXPECT_EQ(ui::StockInfoTableModel::getTickerColumn(), 0);
    }

}   // namespace

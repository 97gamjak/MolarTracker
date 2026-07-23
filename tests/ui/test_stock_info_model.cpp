#include <gtest/gtest.h>

#include <QVariant>

#include "common/finance.hpp"
#include "drafts/stock_draft.hpp"
#include "ui/securities/stock_info_model.hpp"

namespace
{
    drafts::StockInfoDraft makeStock(
        const std::string& ticker,
        const std::string& shortName = "Short Name",
        const std::string& exchange  = "NYSE"
    )
    {
        return drafts::StockInfoDraft{
            ticker,
            shortName,
            "Long Name of " + ticker,
            exchange,
            Currency::USD,
            "Technology",
            "Software",
            AssetClass::Stock
        };
    }

    // -------------------------------------------------------------------------

    class StockInfoModelTest : public ::testing::Test
    {
       protected:
        ui::StockInfoTableModel _model;
    };

    TEST_F(StockInfoModelTest, InitiallyEmpty)
    {
        EXPECT_EQ(_model.rowCount({}), 0);
    }

    TEST_F(StockInfoModelTest, ColumnCountIsSeven)
    {
        EXPECT_EQ(_model.columnCount({}), 7);
    }

    TEST_F(StockInfoModelTest, ValidParentReturnsZeroRows)
    {
        const auto parent = _model.index(0, 0);
        EXPECT_EQ(_model.rowCount(parent), 0);
    }

    TEST_F(StockInfoModelTest, ValidParentReturnsZeroColumns)
    {
        _model.setRows({makeStock("AAPL")});
        const auto parent = _model.index(0, 0);
        ASSERT_TRUE(parent.isValid());
        EXPECT_EQ(_model.columnCount(parent), 0);
    }

    TEST_F(StockInfoModelTest, SetRowsUpdatesRowCount)
    {
        _model.setRows({makeStock("AAPL"), makeStock("MSFT")});
        EXPECT_EQ(_model.rowCount({}), 2);
    }

    TEST_F(StockInfoModelTest, SetRowsReplacesExistingRows)
    {
        _model.setRows({makeStock("AAPL"), makeStock("MSFT")});
        _model.setRows({makeStock("GOOG")});
        EXPECT_EQ(_model.rowCount({}), 1);
    }

    TEST_F(StockInfoModelTest, DataDisplayRoleReturnsTicker)
    {
        _model.setRows({makeStock("AAPL")});
        const auto idx =
            _model.index(0, ui::StockInfoTableModel::getTickerColumn());
        const auto data = _model.data(idx, Qt::DisplayRole);
        EXPECT_EQ(data.toString(), "AAPL");
    }

    TEST_F(StockInfoModelTest, DataDisplayRoleReturnsShortName)
    {
        _model.setRows({makeStock("AAPL", "Apple Inc.")});
        const auto idx  = _model.index(0, 1);
        const auto data = _model.data(idx, Qt::DisplayRole);
        EXPECT_EQ(data.toString(), "Apple Inc.");
    }

    TEST_F(StockInfoModelTest, DataDisplayRoleReturnsExchange)
    {
        _model.setRows({makeStock("AAPL", "Short", "NASDAQ")});
        const auto idx  = _model.index(0, 2);
        const auto data = _model.data(idx, Qt::DisplayRole);
        EXPECT_EQ(data.toString(), "NASDAQ");
    }

    TEST_F(StockInfoModelTest, DataInvalidIndexReturnsNullVariant)
    {
        _model.setRows({makeStock("AAPL")});
        const auto data = _model.data(QModelIndex{}, Qt::DisplayRole);
        EXPECT_FALSE(data.isValid());
    }

    TEST_F(StockInfoModelTest, DataOutOfRangeReturnsNullVariant)
    {
        _model.setRows({makeStock("AAPL")});
        const auto idx  = _model.index(99, 0);
        const auto data = _model.data(idx, Qt::DisplayRole);
        EXPECT_FALSE(data.isValid());
    }

    TEST_F(StockInfoModelTest, DataTextAlignmentRoleReturnsVariant)
    {
        _model.setRows({makeStock("AAPL")});
        const auto idx  = _model.index(0, 0);
        const auto data = _model.data(idx, Qt::TextAlignmentRole);
        EXPECT_TRUE(data.isValid());
    }

    TEST_F(StockInfoModelTest, DataUnknownRoleReturnsNullVariant)
    {
        _model.setRows({makeStock("AAPL")});
        const auto idx  = _model.index(0, 0);
        const auto data = _model.data(idx, Qt::UserRole + 999);
        EXPECT_FALSE(data.isValid());
    }

    TEST_F(StockInfoModelTest, HeaderDataHorizontalDisplayRoleReturnsLabel)
    {
        const auto header =
            _model.headerData(0, Qt::Horizontal, Qt::DisplayRole);
        EXPECT_TRUE(header.isValid());
        EXPECT_FALSE(header.toString().isEmpty());
    }

    TEST_F(StockInfoModelTest, HeaderDataVerticalReturnsNullVariant)
    {
        const auto header = _model.headerData(0, Qt::Vertical, Qt::DisplayRole);
        EXPECT_FALSE(header.isValid());
    }

    TEST_F(StockInfoModelTest, HeaderDataNonDisplayRoleReturnsNullVariant)
    {
        const auto header =
            _model.headerData(0, Qt::Horizontal, Qt::UserRole + 1);
        EXPECT_FALSE(header.isValid());
    }

    TEST_F(StockInfoModelTest, FlagsOnValidIndexAreEnabledAndSelectable)
    {
        _model.setRows({makeStock("AAPL")});
        const auto idx   = _model.index(0, 0);
        const auto flags = _model.flags(idx);
        EXPECT_TRUE((flags & Qt::ItemIsEnabled) != 0);
        EXPECT_TRUE((flags & Qt::ItemIsSelectable) != 0);
    }

    TEST_F(StockInfoModelTest, FlagsOnInvalidIndexAreNoItemFlags)
    {
        const auto flags = _model.flags(QModelIndex{});
        EXPECT_EQ(flags, Qt::NoItemFlags);
    }

    TEST_F(StockInfoModelTest, GetTickerColumnIsZero)
    {
        EXPECT_EQ(ui::StockInfoTableModel::getTickerColumn(), 0);
    }

    TEST_F(StockInfoModelTest, UpdateRowModifiesExistingEntry)
    {
        _model.setRows({makeStock("AAPL", "Apple")});

        const auto updated = makeStock("AAPL", "Apple Inc.");
        _model.updateRow(updated);

        EXPECT_EQ(_model.rowCount({}), 1);
        const auto idx  = _model.index(0, 1);
        const auto data = _model.data(idx, Qt::DisplayRole);
        EXPECT_EQ(data.toString(), "Apple Inc.");
    }

    TEST_F(StockInfoModelTest, UpdateRowAppendsIfTickerNotFound)
    {
        _model.setRows({makeStock("AAPL")});
        _model.updateRow(makeStock("MSFT"));
        EXPECT_EQ(_model.rowCount({}), 2);
    }

}   // namespace

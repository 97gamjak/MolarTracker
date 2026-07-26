#include <gtest/gtest.h>

#include <QVariant>
#include <string>

#include "common/finance.hpp"
#include "common/timestamp.hpp"
#include "config/id_types.hpp"
#include "drafts/position/position_stock_draft.hpp"
#include "drafts/stock_draft.hpp"
#include "ui/position/position_selection_table_model.hpp"

namespace
{
    drafts::StockInfoDraft makeStockInfo(const std::string& ticker)
    {
        const auto shortName = std::string("Short ") + ticker;
        const auto longName  = std::string("Long ") + ticker;
        return drafts::StockInfoDraft{
            ticker,
            shortName,
            longName,
            "NYSE",
            Currency::USD,
            "Technology",
            "Software",
            AssetClass::Stock
        };
    }

    drafts::PositionStockDetailDraft makePosition(
        PositionId         id,
        const std::string& ticker
    )
    {
        const auto quantity = Quantity{100};
        const auto price    = Cash{Currency::USD, 150};
        const auto total    = price * quantity;
        return drafts::PositionStockDetailDraft{
            id,
            makeStockInfo(ticker),
            Timestamp{},
            quantity,
            price,
            total,
            Cash{Currency::USD, 0},
            Percentage{0.0},
            std::nullopt
        };
    }

    // -------------------------------------------------------------------------

    class PositionSelectionTableModelTest : public ::testing::Test
    {
    };

    TEST_F(PositionSelectionTableModelTest, EmptyPositionsHasZeroRows)
    {
        const ui::PositionSelectionTableModel model{{}};
        EXPECT_EQ(model.rowCount({}), 0);
    }

    TEST_F(PositionSelectionTableModelTest, ColumnCountIsThree)
    {
        const ui::PositionSelectionTableModel model{{}};
        // Ticker, Name, OpenedAt
        EXPECT_EQ(model.columnCount({}), 3);
    }

    TEST_F(PositionSelectionTableModelTest, ValidParentReturnsZeroRows)
    {
        const ui::PositionSelectionTableModel model{{}};
        const auto                            parent = model.index(0, 0);
        EXPECT_EQ(model.rowCount(parent), 0);
    }

    TEST_F(PositionSelectionTableModelTest, ValidParentReturnsZeroColumns)
    {
        const ui::PositionSelectionTableModel model{
            {makePosition(PositionId{1}, "AAPL")}
        };
        const auto parent = model.index(0, 0);
        ASSERT_TRUE(parent.isValid());
        EXPECT_EQ(model.columnCount(parent), 0);
    }

    TEST_F(PositionSelectionTableModelTest, RowCountMatchesPositions)
    {
        const ui::PositionSelectionTableModel model{
            {makePosition(PositionId{1}, "AAPL"),
             makePosition(PositionId{2}, "MSFT")}
        };
        EXPECT_EQ(model.rowCount({}), 2);
    }

    TEST_F(PositionSelectionTableModelTest, DataDisplayRoleReturnsTicker)
    {
        const ui::PositionSelectionTableModel model{
            {makePosition(PositionId{1}, "AAPL")}
        };
        const auto idx  = model.index(0, 0);
        const auto data = model.data(idx, Qt::DisplayRole);
        EXPECT_EQ(data.toString(), "AAPL");
    }

    TEST_F(PositionSelectionTableModelTest, DataDisplayRoleReturnsShortName)
    {
        const ui::PositionSelectionTableModel model{
            {makePosition(PositionId{1}, "AAPL")}
        };
        const auto idx  = model.index(0, 1);
        const auto data = model.data(idx, Qt::DisplayRole);
        EXPECT_EQ(data.toString(), "Short AAPL");
    }

    TEST_F(PositionSelectionTableModelTest, DataDisplayRoleDateIsNotEmpty)
    {
        const ui::PositionSelectionTableModel model{
            {makePosition(PositionId{1}, "AAPL")}
        };
        const auto idx  = model.index(0, 2);
        const auto data = model.data(idx, Qt::DisplayRole);
        EXPECT_FALSE(data.toString().isEmpty());
    }

    TEST_F(PositionSelectionTableModelTest, DataInvalidIndexReturnsNullVariant)
    {
        const ui::PositionSelectionTableModel model{{}};
        const auto data = model.data(QModelIndex{}, Qt::DisplayRole);
        EXPECT_FALSE(data.isValid());
    }

    TEST_F(PositionSelectionTableModelTest, DataOutOfRangeReturnsNullVariant)
    {
        const ui::PositionSelectionTableModel model{
            {makePosition(PositionId{1}, "AAPL")}
        };
        const auto idx  = model.index(99, 0);
        const auto data = model.data(idx, Qt::DisplayRole);
        EXPECT_FALSE(data.isValid());
    }

    TEST_F(
        PositionSelectionTableModelTest,
        DataTextAlignmentRoleForTickerColumn
    )
    {
        const ui::PositionSelectionTableModel model{
            {makePosition(PositionId{1}, "AAPL")}
        };
        const auto idx  = model.index(0, 0);
        const auto data = model.data(idx, Qt::TextAlignmentRole);
        EXPECT_TRUE(data.isValid());
    }

    TEST_F(PositionSelectionTableModelTest, HeaderDataHorizontalDisplayRole)
    {
        const ui::PositionSelectionTableModel model{{}};
        const auto                            header =
            model.headerData(0, Qt::Horizontal, Qt::DisplayRole);
        EXPECT_TRUE(header.isValid());
        EXPECT_FALSE(header.toString().isEmpty());
    }

    TEST_F(
        PositionSelectionTableModelTest,
        HeaderDataVerticalReturnsNullVariant
    )
    {
        const ui::PositionSelectionTableModel model{{}};
        const auto header = model.headerData(0, Qt::Vertical, Qt::DisplayRole);
        EXPECT_FALSE(header.isValid());
    }

    TEST_F(PositionSelectionTableModelTest, PositionAtValidRowReturnsPosition)
    {
        const ui::PositionSelectionTableModel model{
            {makePosition(PositionId{42}, "AAPL")}
        };
        const auto pos = model.positionAt(0);
        ASSERT_TRUE(pos.has_value());
        EXPECT_EQ(pos->getPositionId(), PositionId{42});
    }

    TEST_F(PositionSelectionTableModelTest, PositionAtNegativeRowReturnsNullopt)
    {
        const ui::PositionSelectionTableModel model{
            {makePosition(PositionId{1}, "AAPL")}
        };
        EXPECT_FALSE(model.positionAt(-1).has_value());
    }

    TEST_F(PositionSelectionTableModelTest, PositionAtOutOfRangeReturnsNullopt)
    {
        const ui::PositionSelectionTableModel model{
            {makePosition(PositionId{1}, "AAPL")}
        };
        EXPECT_FALSE(model.positionAt(99).has_value());
    }

    TEST_F(PositionSelectionTableModelTest, PositionAtEmptyModelReturnsNullopt)
    {
        const ui::PositionSelectionTableModel model{{}};
        EXPECT_FALSE(model.positionAt(0).has_value());
    }

}   // namespace

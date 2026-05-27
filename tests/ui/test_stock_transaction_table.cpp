// test_stock_transaction_table.cpp
//
// GoogleTest-based tests for ui::StockTransactionTableModel.
//
// Coverage:
//  - empty model: rowCount=0, columnCount, headerData
//  - setTransactions populates rows
//  - data() Date column returns formatted date
//  - data() Stock column returns ticker from first leg
//  - data() Account column looks up leg account name
//  - data() for invalid / out-of-range index returns empty
//  - flags: enabled + selectable; invalid returns NoItemFlags
//  - getDescriptionIndex / getDateIndex return correct indices
//  - TextAlignmentRole returns a valid variant

#include <gtest/gtest.h>

#include <QVariant>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "config/quantity.hpp"
#include "drafts/transaction_draft.hpp"
#include "finance/cash.hpp"
#include "ui/transaction/stock_transaction_table.hpp"
#include "utils/timestamp.hpp"

namespace
{

    using IdToNameMap =
        std::unordered_map<AccountId, std::string, AccountId::Hash>;

    drafts::TransactionOverviewDraft makeStockTransaction(
        AccountId          legAccountId,
        AccountId          entryAccountId,
        const std::string& ticker  = "AAPL",
        micro_units        qty     = 1'00000000,
        const std::string& comment = ""
    )
    {
        const Timestamp ts{Clock::now()};
        const finance::Cash price{Currency::USD, 15000};
        const finance::Cash cash{Currency::USD, 15000};
        const Quantity      quantity{qty};

        drafts::TradeLegDraft leg{legAccountId, price, quantity, ticker};

        std::vector<drafts::TransactionEntryDraft> entries;
        entries.emplace_back(entryAccountId, cash);

        std::vector<drafts::TradeLegDraft> legs;
        legs.push_back(leg);

        return drafts::TransactionOverviewDraft{
            TransactionDataType::Trade,
            ts,
            std::move(entries),
            std::move(legs),
            comment.empty() ? std::nullopt
                            : std::make_optional<std::string>(comment)
        };
    }

    class StockTransactionTableTest : public ::testing::Test
    {
       protected:
        ui::StockTransactionTableModel _model;

        const AccountId _legAccount{AccountId::from(10)};
        const AccountId _entryAccount{AccountId::from(11)};
    };

    TEST_F(StockTransactionTableTest, EmptyModelHasZeroRows)
    {
        EXPECT_EQ(_model.rowCount({}), 0);
    }

    TEST_F(StockTransactionTableTest, ColumnCountIsGreaterThanZero)
    {
        EXPECT_GT(_model.columnCount({}), 0);
    }

    TEST_F(StockTransactionTableTest, ColumnCountZeroForValidParent)
    {
        _model.setTransactions(
            {makeStockTransaction(_legAccount, _entryAccount)},
            {}
        );
        const auto idx = _model.index(0, 0, {});
        EXPECT_EQ(_model.columnCount(idx), 0);
    }

    TEST_F(StockTransactionTableTest, SetTransactionsUpdatesRowCount)
    {
        _model.setTransactions(
            {makeStockTransaction(_legAccount, _entryAccount),
             makeStockTransaction(_legAccount, _entryAccount, "MSFT")},
            {}
        );
        EXPECT_EQ(_model.rowCount({}), 2);
    }

    TEST_F(StockTransactionTableTest, HeaderDateColumn)
    {
        const auto h = _model.headerData(
            _model.getDateIndex(), Qt::Horizontal, Qt::DisplayRole
        );
        EXPECT_EQ(h.toString(), "Date");
    }

    TEST_F(StockTransactionTableTest, HeaderDataVerticalReturnsEmpty)
    {
        const auto h = _model.headerData(0, Qt::Vertical, Qt::DisplayRole);
        EXPECT_FALSE(h.isValid());
    }

    TEST_F(StockTransactionTableTest, DataDateColumnFormattedCorrectly)
    {
        _model.setTransactions(
            {makeStockTransaction(_legAccount, _entryAccount)},
            {}
        );
        const auto idx = _model.index(0, _model.getDateIndex(), {});
        const auto s   = _model.data(idx, Qt::DisplayRole).toString();
        EXPECT_EQ(s.length(), 10);
    }

    TEST_F(StockTransactionTableTest, DataStockColumnReturnsTicker)
    {
        _model.setTransactions(
            {makeStockTransaction(_legAccount, _entryAccount, "TSLA")},
            {}
        );
        // Stock column is 2 (Date=0, Type=1, Stock=2)
        const auto idx = _model.index(0, 2, {});
        EXPECT_EQ(_model.data(idx, Qt::DisplayRole).toString(), "TSLA");
    }

    TEST_F(StockTransactionTableTest, DataAccountColumnReturnsName)
    {
        IdToNameMap nameMap;
        nameMap[_legAccount] = "Brokerage";

        _model.setTransactions(
            {makeStockTransaction(_legAccount, _entryAccount)},
            std::move(nameMap)
        );
        // Account column is 3 (Date=0, Type=1, Stock=2, Account=3)
        const auto idx = _model.index(0, 3, {});
        EXPECT_EQ(_model.data(idx, Qt::DisplayRole).toString(), "Brokerage");
    }

    TEST_F(StockTransactionTableTest, DataInvalidIndexReturnsEmpty)
    {
        EXPECT_FALSE(_model.data({}, Qt::DisplayRole).isValid());
    }

    TEST_F(StockTransactionTableTest, DataOutOfRangeReturnsEmpty)
    {
        _model.setTransactions(
            {makeStockTransaction(_legAccount, _entryAccount)},
            {}
        );
        const auto idx = _model.index(99, 0, {});
        EXPECT_FALSE(_model.data(idx, Qt::DisplayRole).isValid());
    }

    TEST_F(StockTransactionTableTest, TextAlignmentRoleReturnsVariant)
    {
        _model.setTransactions(
            {makeStockTransaction(_legAccount, _entryAccount)},
            {}
        );
        const auto idx = _model.index(0, 0, {});
        EXPECT_TRUE(_model.data(idx, Qt::TextAlignmentRole).isValid());
    }

    TEST_F(StockTransactionTableTest, FlagsValidIndex)
    {
        _model.setTransactions(
            {makeStockTransaction(_legAccount, _entryAccount)},
            {}
        );
        const auto idx   = _model.index(0, 0, {});
        const auto flags = _model.flags(idx);
        EXPECT_TRUE(flags & Qt::ItemIsEnabled);
        EXPECT_TRUE(flags & Qt::ItemIsSelectable);
    }

    TEST_F(StockTransactionTableTest, FlagsInvalidIndex)
    {
        EXPECT_EQ(_model.flags({}), Qt::NoItemFlags);
    }

    TEST_F(StockTransactionTableTest, GetDescriptionIndexIsNonNegative)
    {
        EXPECT_GE(_model.getDescriptionIndex(), 0);
    }

    TEST_F(StockTransactionTableTest, GetDateIndexIsZero)
    {
        EXPECT_EQ(_model.getDateIndex(), 0);
    }

    TEST_F(StockTransactionTableTest, DescriptionColumnReturnsComment)
    {
        _model.setTransactions(
            {makeStockTransaction(_legAccount, _entryAccount, "AAPL", 100, "Buy order")},
            {}
        );
        const auto idx = _model.index(0, _model.getDescriptionIndex(), {});
        EXPECT_EQ(_model.data(idx, Qt::DisplayRole).toString(), "Buy order");
    }

    TEST_F(StockTransactionTableTest, ClearViaEmptySetTransactions)
    {
        _model.setTransactions(
            {makeStockTransaction(_legAccount, _entryAccount)},
            {}
        );
        ASSERT_EQ(_model.rowCount({}), 1);
        _model.setTransactions({}, {});
        EXPECT_EQ(_model.rowCount({}), 0);
    }

}   // namespace

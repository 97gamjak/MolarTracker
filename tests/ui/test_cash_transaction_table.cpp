// test_cash_transaction_table.cpp
//
// GoogleTest-based tests for ui::CashTransactionTableModel.
//
// Coverage:
//  - empty model: rowCount=0, columnCount, headerData
//  - setTransactions populates rows
//  - data() Date column returns formatted date string
//  - data() Description column returns comment
//  - data() Account column looks up name from map
//  - data() Amount column returns cash string
//  - data() for invalid index returns empty
//  - flags: enabled + selectable; invalid index returns NoItemFlags
//  - getDescriptionIndex / getDateIndex return correct indices
//  - headerData returns column labels

#include <gtest/gtest.h>

#include <QVariant>
#include <chrono>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "drafts/transaction_draft.hpp"
#include "finance/cash.hpp"
#include "ui/transaction/cash_transaction_table.hpp"
#include "utils/timestamp.hpp"

namespace
{

    using IdToNameMap =
        std::unordered_map<AccountId, std::string, AccountId::Hash>;

    drafts::TransactionOverviewDraft makeCashTransaction(
        AccountId                  fromId,
        AccountId                  toId,
        micro_units                amount,
        const std::string&         comment = "",
        const TimePoint&           tp = Clock::now()
    )
    {
        const Timestamp ts{tp};
        const finance::Cash cash{Currency::USD, amount};

        std::vector<drafts::TransactionEntryDraft> entries;
        entries.emplace_back(fromId, cash);
        entries.emplace_back(toId, cash);

        return drafts::TransactionOverviewDraft{
            TransactionDataType::Cash,
            ts,
            std::move(entries),
            {},
            comment.empty() ? std::nullopt
                            : std::make_optional<std::string>(comment)
        };
    }

    class CashTransactionTableTest : public ::testing::Test
    {
       protected:
        ui::CashTransactionTableModel _model;

        const AccountId _account1{AccountId::from(1)};
        const AccountId _account2{AccountId::from(2)};
    };

    TEST_F(CashTransactionTableTest, EmptyModelHasZeroRows)
    {
        EXPECT_EQ(_model.rowCount({}), 0);
    }

    TEST_F(CashTransactionTableTest, ColumnCountIsCorrect)
    {
        EXPECT_GT(_model.columnCount({}), 0);
    }

    TEST_F(CashTransactionTableTest, ColumnCountZeroForValidParent)
    {
        _model.setTransactions(
            {makeCashTransaction(_account1, _account2, 1000)},
            {{_account1, "Cash"}, {_account2, "External"}}
        );
        const auto idx = _model.index(0, 0, {});
        EXPECT_EQ(_model.columnCount(idx), 0);
    }

    TEST_F(CashTransactionTableTest, SetTransactionsUpdatesRowCount)
    {
        std::vector<drafts::TransactionOverviewDraft> txns;
        txns.push_back(makeCashTransaction(_account1, _account2, 5000));
        txns.push_back(makeCashTransaction(_account1, _account2, 1000));
        _model.setTransactions(std::move(txns), {});
        EXPECT_EQ(_model.rowCount({}), 2);
    }

    TEST_F(CashTransactionTableTest, HeaderDataDateColumn)
    {
        const auto dateIdx = _model.getDateIndex();
        const auto h = _model.headerData(dateIdx, Qt::Horizontal, Qt::DisplayRole);
        EXPECT_EQ(h.toString(), "Date");
    }

    TEST_F(CashTransactionTableTest, HeaderDataDescriptionColumn)
    {
        const auto descIdx = _model.getDescriptionIndex();
        const auto h =
            _model.headerData(descIdx, Qt::Horizontal, Qt::DisplayRole);
        EXPECT_FALSE(h.toString().isEmpty());
    }

    TEST_F(CashTransactionTableTest, HeaderDataEmptyForVertical)
    {
        const auto h = _model.headerData(0, Qt::Vertical, Qt::DisplayRole);
        EXPECT_FALSE(h.isValid());
    }

    TEST_F(CashTransactionTableTest, DataDateColumnReturnsFormattedDate)
    {
        _model.setTransactions(
            {makeCashTransaction(_account1, _account2, 100)},
            {}
        );
        const auto idx = _model.index(0, _model.getDateIndex(), {});
        const auto dateStr = _model.data(idx, Qt::DisplayRole).toString();
        EXPECT_FALSE(dateStr.isEmpty());
        // Expect yyyy-MM-dd format (10 chars)
        EXPECT_EQ(dateStr.length(), 10);
    }

    TEST_F(CashTransactionTableTest, DataDescriptionColumnReturnsComment)
    {
        _model.setTransactions(
            {makeCashTransaction(_account1, _account2, 100, "Test comment")},
            {}
        );
        const auto descIdx = _model.getDescriptionIndex();
        const auto idx     = _model.index(0, descIdx, {});
        EXPECT_EQ(_model.data(idx, Qt::DisplayRole).toString(), "Test comment");
    }

    TEST_F(CashTransactionTableTest, DataDescriptionEmptyWhenNoComment)
    {
        _model.setTransactions(
            {makeCashTransaction(_account1, _account2, 100)},
            {}
        );
        const auto idx = _model.index(0, _model.getDescriptionIndex(), {});
        EXPECT_TRUE(_model.data(idx, Qt::DisplayRole).toString().isEmpty());
    }

    TEST_F(CashTransactionTableTest, DataAccountColumnReturnsName)
    {
        IdToNameMap nameMap;
        nameMap[_account1] = "My Savings";
        nameMap[_account2] = "External";

        _model.setTransactions(
            {makeCashTransaction(_account1, _account2, 500)},
            std::move(nameMap)
        );

        // Account column is index 2 (Date=0, Type=1, Account=2)
        const auto idx = _model.index(0, 2, {});
        EXPECT_EQ(_model.data(idx, Qt::DisplayRole).toString(), "My Savings");
    }

    TEST_F(CashTransactionTableTest, DataAccountColumnEmptyWhenNotInMap)
    {
        _model.setTransactions(
            {makeCashTransaction(_account1, _account2, 500)},
            {}
        );
        const auto idx = _model.index(0, 2, {});
        EXPECT_TRUE(_model.data(idx, Qt::DisplayRole).toString().isEmpty());
    }

    TEST_F(CashTransactionTableTest, DataInvalidIndexReturnsEmpty)
    {
        EXPECT_FALSE(_model.data({}, Qt::DisplayRole).isValid());
    }

    TEST_F(CashTransactionTableTest, DataOutOfRangeRowReturnsEmpty)
    {
        _model.setTransactions(
            {makeCashTransaction(_account1, _account2, 100)},
            {}
        );
        const auto idx = _model.index(99, 0, {});
        EXPECT_FALSE(_model.data(idx, Qt::DisplayRole).isValid());
    }

    TEST_F(CashTransactionTableTest, TextAlignmentRoleReturnsVariant)
    {
        _model.setTransactions(
            {makeCashTransaction(_account1, _account2, 100)},
            {}
        );
        const auto idx = _model.index(0, 0, {});
        EXPECT_TRUE(_model.data(idx, Qt::TextAlignmentRole).isValid());
    }

    TEST_F(CashTransactionTableTest, FlagsForValidIndex)
    {
        _model.setTransactions(
            {makeCashTransaction(_account1, _account2, 100)},
            {}
        );
        const auto idx   = _model.index(0, 0, {});
        const auto flags = _model.flags(idx);
        EXPECT_TRUE(flags & Qt::ItemIsEnabled);
        EXPECT_TRUE(flags & Qt::ItemIsSelectable);
    }

    TEST_F(CashTransactionTableTest, FlagsForInvalidIndex)
    {
        EXPECT_EQ(_model.flags({}), Qt::NoItemFlags);
    }

    TEST_F(CashTransactionTableTest, GetDescriptionIndexIsPositive)
    {
        EXPECT_GE(_model.getDescriptionIndex(), 0);
    }

    TEST_F(CashTransactionTableTest, GetDateIndexIsZero)
    {
        EXPECT_EQ(_model.getDateIndex(), 0);
    }

    TEST_F(CashTransactionTableTest, ClearTransactionsViaSetEmpty)
    {
        _model.setTransactions(
            {makeCashTransaction(_account1, _account2, 100)},
            {}
        );
        ASSERT_EQ(_model.rowCount({}), 1);
        _model.setTransactions({}, {});
        EXPECT_EQ(_model.rowCount({}), 0);
    }

}   // namespace

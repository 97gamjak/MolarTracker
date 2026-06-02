#include <gtest/gtest.h>

#include <QVariant>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "config/quantity.hpp"
#include "drafts/transaction_draft.hpp"
#include "finance/cash.hpp"
#include "ui/transaction/cash_transaction_table.hpp"
#include "ui/transaction/stock_transaction_table.hpp"
#include "utils/timestamp.hpp"

namespace
{
    drafts::TransactionEntryDraft makeEntry(
        AccountId   accountId  = AccountId{1},
        bool        isExternal = false,
        micro_units amount     = 10000,
        Currency    currency   = Currency::USD
    )
    {
        return drafts::TransactionEntryDraft{
            accountId,
            finance::Cash{currency, amount},
            TransactionEntryType::General,
            isExternal
        };
    }

    drafts::TransactionOverviewDraft makeCashTx(
        const std::optional<std::string>& comment = std::nullopt
    )
    {
        return drafts::TransactionOverviewDraft{
            TransactionDataType::Cash,
            Timestamp{},
            {makeEntry()},
            {},
            comment
        };
    }

    drafts::TransactionOverviewDraft makeStockTx()
    {
        auto leg = drafts::TradeLegDraft{
            AccountId{2},
            finance::Cash{Currency::USD, 15000},
            Quantity{100'000'000},
            "AAPL"
        };
        return drafts::TransactionOverviewDraft{
            TransactionDataType::Trade,
            Timestamp{},
            {makeEntry()},
            {leg},
            std::nullopt
        };
    }

    // -------------------------------------------------------------------------
    // CashTransactionTableModel
    // -------------------------------------------------------------------------

    class CashTransactionTableModelTest : public ::testing::Test
    {
       protected:
        ui::CashTransactionTableModel _model;
    };

    TEST_F(CashTransactionTableModelTest, InitiallyEmpty)
    {
        EXPECT_EQ(_model.rowCount({}), 0);
    }

    TEST_F(CashTransactionTableModelTest, ColumnCountIsSeven)
    {
        // Date, Type, Account, ReferenceAccount, Amount, Fees, Description
        EXPECT_EQ(_model.columnCount({}), 7);
    }

    TEST_F(CashTransactionTableModelTest, ValidParentReturnsZeroColumns)
    {
        _model.setTransactions({makeCashTx()}, {});
        const auto parent = _model.index(0, 0);
        ASSERT_TRUE(parent.isValid());
        EXPECT_EQ(_model.columnCount(parent), 0);
    }

    TEST_F(CashTransactionTableModelTest, DateIndexIsZero)
    {
        EXPECT_EQ(_model.getDateIndex(), 0);
    }

    TEST_F(CashTransactionTableModelTest, DescriptionIndexIsSix)
    {
        EXPECT_EQ(_model.getDescriptionIndex(), 6);
    }

    TEST_F(CashTransactionTableModelTest, SetTransactionsUpdatesRowCount)
    {
        _model.setTransactions({makeCashTx(), makeCashTx()}, {});
        EXPECT_EQ(_model.rowCount({}), 2);
    }

    TEST_F(CashTransactionTableModelTest, SetTransactionsReplacesExisting)
    {
        _model.setTransactions({makeCashTx(), makeCashTx()}, {});
        _model.setTransactions({makeCashTx()}, {});
        EXPECT_EQ(_model.rowCount({}), 1);
    }

    TEST_F(CashTransactionTableModelTest, DataInvalidIndexReturnsNullVariant)
    {
        const auto data = _model.data(QModelIndex{}, Qt::DisplayRole);
        EXPECT_FALSE(data.isValid());
    }

    TEST_F(CashTransactionTableModelTest, DataOutOfRangeReturnsNullVariant)
    {
        _model.setTransactions({makeCashTx()}, {});
        const auto idx  = _model.index(99, 0);
        const auto data = _model.data(idx, Qt::DisplayRole);
        EXPECT_FALSE(data.isValid());
    }

    TEST_F(CashTransactionTableModelTest, DataDisplayRoleDescriptionColumn)
    {
        _model.setTransactions({makeCashTx("My Note")}, {});
        const auto idx  = _model.index(0, _model.getDescriptionIndex());
        const auto data = _model.data(idx, Qt::DisplayRole);
        EXPECT_EQ(data.toString(), "My Note");
    }

    TEST_F(CashTransactionTableModelTest, DataDisplayRoleDateColumnIsNotEmpty)
    {
        _model.setTransactions({makeCashTx()}, {});
        const auto idx  = _model.index(0, _model.getDateIndex());
        const auto data = _model.data(idx, Qt::DisplayRole);
        EXPECT_FALSE(data.toString().isEmpty());
    }

    TEST_F(CashTransactionTableModelTest, DataTextAlignmentRoleReturnsVariant)
    {
        _model.setTransactions({makeCashTx()}, {});
        const auto idx  = _model.index(0, 0);
        const auto data = _model.data(idx, Qt::TextAlignmentRole);
        EXPECT_TRUE(data.isValid());
    }

    TEST_F(CashTransactionTableModelTest, DataUnknownRoleReturnsNullVariant)
    {
        _model.setTransactions({makeCashTx()}, {});
        const auto idx  = _model.index(0, 0);
        const auto data = _model.data(idx, Qt::UserRole + 999);
        EXPECT_FALSE(data.isValid());
    }

    TEST_F(CashTransactionTableModelTest, HeaderDataHorizontalDisplayRole)
    {
        const auto header =
            _model.headerData(0, Qt::Horizontal, Qt::DisplayRole);
        EXPECT_TRUE(header.isValid());
        EXPECT_FALSE(header.toString().isEmpty());
    }

    TEST_F(CashTransactionTableModelTest, HeaderDataVerticalReturnsNullVariant)
    {
        const auto header = _model.headerData(0, Qt::Vertical, Qt::DisplayRole);
        EXPECT_FALSE(header.isValid());
    }

    TEST_F(CashTransactionTableModelTest, FlagsOnValidIndexEnabledAndSelectable)
    {
        _model.setTransactions({makeCashTx()}, {});
        const auto idx   = _model.index(0, 0);
        const auto flags = _model.flags(idx);
        EXPECT_TRUE((flags & Qt::ItemIsEnabled) != 0);
        EXPECT_TRUE((flags & Qt::ItemIsSelectable) != 0);
    }

    TEST_F(CashTransactionTableModelTest, FlagsOnInvalidIndexAreNoItemFlags)
    {
        const auto flags = _model.flags(QModelIndex{});
        EXPECT_EQ(flags, Qt::NoItemFlags);
    }

    // -------------------------------------------------------------------------
    // StockTransactionTableModel
    // -------------------------------------------------------------------------

    class StockTransactionTableModelTest : public ::testing::Test
    {
       protected:
        ui::StockTransactionTableModel _model;
    };

    TEST_F(StockTransactionTableModelTest, InitiallyEmpty)
    {
        EXPECT_EQ(_model.rowCount({}), 0);
    }

    TEST_F(StockTransactionTableModelTest, ColumnCountIsNine)
    {
        // Date, Type, Stock, Account, ReferenceAccount, Quantity, Price, Fees,
        // Description
        EXPECT_EQ(_model.columnCount({}), 9);
    }

    TEST_F(StockTransactionTableModelTest, ValidParentReturnsZeroColumns)
    {
        _model.setTransactions({makeStockTx()}, {});
        const auto parent = _model.index(0, 0);
        ASSERT_TRUE(parent.isValid());
        EXPECT_EQ(_model.columnCount(parent), 0);
    }

    TEST_F(StockTransactionTableModelTest, DateIndexIsZero)
    {
        EXPECT_EQ(_model.getDateIndex(), 0);
    }

    TEST_F(StockTransactionTableModelTest, DescriptionIndexIsEight)
    {
        EXPECT_EQ(_model.getDescriptionIndex(), 8);
    }

    TEST_F(StockTransactionTableModelTest, SetTransactionsUpdatesRowCount)
    {
        _model.setTransactions({makeStockTx()}, {});
        EXPECT_EQ(_model.rowCount({}), 1);
    }

    TEST_F(StockTransactionTableModelTest, DataInvalidIndexReturnsNullVariant)
    {
        const auto data = _model.data(QModelIndex{}, Qt::DisplayRole);
        EXPECT_FALSE(data.isValid());
    }

    TEST_F(StockTransactionTableModelTest, HeaderDataHorizontalDisplayRole)
    {
        const auto header =
            _model.headerData(0, Qt::Horizontal, Qt::DisplayRole);
        EXPECT_TRUE(header.isValid());
        EXPECT_FALSE(header.toString().isEmpty());
    }

    TEST_F(
        StockTransactionTableModelTest,
        FlagsOnValidIndexEnabledAndSelectable
    )
    {
        _model.setTransactions({makeStockTx()}, {});
        const auto idx   = _model.index(0, 0);
        const auto flags = _model.flags(idx);
        EXPECT_TRUE((flags & Qt::ItemIsEnabled) != 0);
        EXPECT_TRUE((flags & Qt::ItemIsSelectable) != 0);
    }

}   // namespace

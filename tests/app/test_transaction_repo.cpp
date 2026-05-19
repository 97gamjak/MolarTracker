// test_transaction_repo.cpp
//
// GoogleTest-based integration tests for app::TransactionRepo.
//
// Coverage:
//  - getTransactions() on empty database
//  - addTransaction() returns a valid, auto-incremented ID
//  - addTransaction() round-trips a Cash transaction with one entry
//  - addTransaction() round-trips a Cash transaction with multiple entries
//  - addTransaction() preserves an optional comment
//  - addTransaction() preserves a NULL comment
//  - addTransaction() round-trips a Trade transaction with legs
//  - addTransaction() persists multiple independent transactions
//
// Each test uses its own temp SQLite database for full isolation.
// Prerequisite rows (profile, account, instrument) are inserted via raw SQL
// to bypass ORM insert-policy guards (requires_paired_insert_t).

#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <optional>
#include <random>
#include <string>
#include <vector>

#include "app/migration/migration_runner.hpp"
#include "app/repos/transaction_repo.hpp"
#include "app/repos_api/i_transaction_repo.hpp"
#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "config/quantity.hpp"
#include "db/database.hpp"
#include "finance/cash.hpp"
#include "finance/trade_data.hpp"
#include "finance/transaction.hpp"
#include "finance/transaction_entry.hpp"
#include "finance/transaction_filter.hpp"
#include "utils/timestamp.hpp"

namespace
{
    std::filesystem::path unique_temp_db_path()
    {
        const auto tmp = std::filesystem::temp_directory_path();

        std::random_device                           random;
        std::mt19937_64                              gen(random());
        std::uniform_int_distribution<std::uint64_t> dis;

        return tmp /
               ("molartracker_txrepo_" + std::to_string(dis(gen)) + ".sqlite");
    }

    struct TempDbFile
    {
        std::filesystem::path path;

        explicit TempDbFile(std::filesystem::path path) : path(std::move(path))
        {
        }

        ~TempDbFile()
        {
            std::error_code errorCode;
            std::filesystem::remove(path, errorCode);
            std::filesystem::remove(
                std::filesystem::path(path.string() + ".bck"),
                errorCode
            );
        }

        TempDbFile(const TempDbFile&)            = delete;
        TempDbFile& operator=(const TempDbFile&) = delete;
        TempDbFile(TempDbFile&&)                 = delete;
        TempDbFile& operator=(TempDbFile&&)      = delete;
    };

    // Fixed epoch value used in all round-trip assertions.
    // Using fromInt64 / toInt64 avoids sub-millisecond precision loss.
    constexpr std::int64_t TEST_TS = 1'715'000'000'000LL;

    class TransactionRepoFixture : public ::testing::Test
    {
       protected:
        // Seeded via raw SQL in SetUp; IDs are deterministic (first inserts).
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        const AccountId    _accountId{1};
        const InstrumentId _instrumentId{1};
        const PositionId   _positionId{1};

        TempDbFile           _tempFile{unique_temp_db_path()};
        db::Database         _db{_tempFile.path};
        app::MigrationRunner _migrationRunner{_db};
        app::TransactionRepo _repo{_db};
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        void SetUp() override
        {
            // Raw SQL bypasses requires_paired_insert_t ORM policy.
            _db.execute(
                "INSERT INTO profile (name, email) "
                "VALUES ('TestProfile', NULL)"
            );
            _db.execute(
                "INSERT INTO account (kind, profile_id, name, status, "
                "currency) VALUES (0, 1, 'TestAccount', 0, 0)"
            );
            _db.execute("INSERT INTO instrument (id) VALUES (NULL)");
            _db.execute("INSERT INTO position (opened_at) VALUES (1)");
        }

        [[nodiscard]] finance::Transaction makeCashTx(
            std::optional<std::string> comment = std::nullopt,
            micro_units                amount  = 100'000LL
        ) const
        {
            return finance::Transaction{
                TransactionId::invalid(),
                Timestamp::fromInt64(TEST_TS),
                TransactionStatus::Completed,
                finance::CashData{},
                {finance::TransactionEntry{
                    TransactionEntryId::invalid(),
                    _accountId,
                    finance::Cash{Currency::USD, amount}
                }},
                std::move(comment)
            };
        }

        [[nodiscard]] finance::Transaction makeTradeTx() const
        {
            constexpr auto     quantity = 100'000'000LL;   // 1.0 in micro-units
            constexpr auto     price = 150'000'000LL;   // $1.50 in micro-units
            finance::TradeData data;
            data.addLeg(
                finance::TradeLeg{
                    _accountId,
                    _instrumentId,
                    Quantity{quantity},
                    finance::Cash{Currency::USD, price},
                    _positionId
                }
            );

            constexpr auto price2 = -15'000'000'000LL;
            return finance::Transaction{
                TransactionId::invalid(),
                Timestamp::fromInt64(TEST_TS),
                TransactionStatus::Completed,
                data,
                {finance::TransactionEntry{
                    TransactionEntryId::invalid(),
                    _accountId,
                    finance::Cash{Currency::USD, price2}
                }},
                "trade comment"
            };
        }
    };

}   // namespace

// ---------------------------------------------------------------------------
// getTransactions
// ---------------------------------------------------------------------------

TEST_F(TransactionRepoFixture, GetTransactionsEmptyDatabaseReturnsEmpty)
{
    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    EXPECT_TRUE(txs.empty());
}

// ---------------------------------------------------------------------------
// addTransaction — returned ID
// ---------------------------------------------------------------------------

TEST_F(TransactionRepoFixture, AddTransactionCashReturnsPositiveId)
{
    const auto id = _repo.addTransaction(makeCashTx());

    EXPECT_TRUE(id.isValid());
    EXPECT_GT(id.value(), 0);
}

TEST_F(
    TransactionRepoFixture,
    AddTransactionCashTwoTransactionsIdsAreDistinctAndIncreasing
)
{
    const auto id1 = _repo.addTransaction(makeCashTx());
    const auto id2 = _repo.addTransaction(makeCashTx());

    EXPECT_NE(id1, id2);
    EXPECT_LT(id1.value(), id2.value());
}

// ---------------------------------------------------------------------------
// addTransaction — Cash round-trip
// ---------------------------------------------------------------------------

TEST_F(TransactionRepoFixture, AddTransactionCashSingleTransactionRetrieved)
{
    const auto result = _repo.addTransaction(makeCashTx());

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    ASSERT_EQ(txs.size(), 1U);
}

TEST_F(TransactionRepoFixture, AddTransactionCashTransactionTypeIsDataTypeCash)
{
    const auto result = _repo.addTransaction(makeCashTx());

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    ASSERT_EQ(txs.size(), 1U);
    EXPECT_EQ(txs[0].getType(), TransactionDataType::Cash);
}

TEST_F(TransactionRepoFixture, AddTransactionCashStatusIsPreserved)
{
    const auto result = _repo.addTransaction(makeCashTx());

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    ASSERT_EQ(txs.size(), 1U);
    EXPECT_EQ(txs[0].getStatus(), TransactionStatus::Completed);
}

TEST_F(TransactionRepoFixture, AddTransactionCashTimestampIsPreserved)
{
    const auto result = _repo.addTransaction(makeCashTx());

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    ASSERT_EQ(txs.size(), 1U);
    EXPECT_EQ(txs[0].getTimestamp().toInt64(), TEST_TS);
}

TEST_F(TransactionRepoFixture, AddTransactionCashAssignedIdMatchesReturnedId)
{
    const auto insertedId = _repo.addTransaction(makeCashTx());

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    ASSERT_EQ(txs.size(), 1U);
    EXPECT_EQ(txs[0].getId(), insertedId);
}

// ---------------------------------------------------------------------------
// addTransaction — comment field
// ---------------------------------------------------------------------------

TEST_F(TransactionRepoFixture, AddTransactionWithCommentCommentIsPreserved)
{
    const auto result = _repo.addTransaction(makeCashTx("my note"));

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    ASSERT_EQ(txs.size(), 1U);
    ASSERT_TRUE(txs[0].getComment().has_value());
    EXPECT_EQ(*txs[0].getComment(), "my note");
}

TEST_F(TransactionRepoFixture, AddTransactionWithNullCommentCommentIsAbsent)
{
    const auto result = _repo.addTransaction(makeCashTx(std::nullopt));

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    ASSERT_EQ(txs.size(), 1U);
    EXPECT_FALSE(txs[0].getComment().has_value());
}

// ---------------------------------------------------------------------------
// addTransaction — entries
// ---------------------------------------------------------------------------

TEST_F(TransactionRepoFixture, AddTransactionCashSingleEntryEntryIsRetrieved)
{
    const auto result =
        _repo.addTransaction(makeCashTx(std::nullopt, 250'000LL));

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    ASSERT_EQ(txs.size(), 1U);
    ASSERT_EQ(txs[0].getEntries().size(), 1U);

    const auto& entry = txs[0].getEntries()[0];
    EXPECT_EQ(entry.getAccountId(), _accountId);
    EXPECT_EQ(entry.getAmount(), 250'000LL);
    EXPECT_EQ(entry.getCurrency(), Currency::USD);
}

TEST_F(
    TransactionRepoFixture,
    AddTransactionCashMultipleEntriesAllEntriesRetrieved
)
{
    const auto price1 = 100'000LL;   // $1.00 in micro-units
    const auto price2 = 200'000LL;   // $2.00 in

    finance::Transaction transaction{
        TransactionId::invalid(),
        Timestamp::fromInt64(TEST_TS),
        TransactionStatus::Completed,
        finance::CashData{},
        {finance::TransactionEntry{
             TransactionEntryId::invalid(),
             _accountId,
             finance::Cash{Currency::USD, price1}
         },
         finance::TransactionEntry{
             TransactionEntryId::invalid(),
             _accountId,
             finance::Cash{Currency::EUR, price2}
         }},
        std::nullopt
    };

    const auto result = _repo.addTransaction(transaction);

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    ASSERT_EQ(txs.size(), 1U);
    EXPECT_EQ(txs[0].getEntries().size(), 2U);
}

// ---------------------------------------------------------------------------
// addTransaction — multiple transactions
// ---------------------------------------------------------------------------

TEST_F(
    TransactionRepoFixture,
    AddTransactionMultipleTransactionsAllAreRetrieved
)
{
    auto result = _repo.addTransaction(makeCashTx("first"));
    result      = _repo.addTransaction(makeCashTx("second"));
    result      = _repo.addTransaction(makeCashTx("third"));

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    EXPECT_EQ(txs.size(), 3U);
}

// ---------------------------------------------------------------------------
// addTransaction — Trade round-trip
// ---------------------------------------------------------------------------

TEST_F(TransactionRepoFixture, AddTransactionTradeTypeIsDataTypeTrade)
{
    const auto result = _repo.addTransaction(makeTradeTx());

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    ASSERT_EQ(txs.size(), 1U);
    EXPECT_EQ(txs[0].getType(), TransactionDataType::Trade);
}

TEST_F(TransactionRepoFixture, AddTransactionTradeLegIsRetrieved)
{
    const auto result = _repo.addTransaction(makeTradeTx());

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    ASSERT_EQ(txs.size(), 1U);

    const auto& data = std::get<finance::TradeData>(txs[0].getData());
    ASSERT_EQ(data.getLegs().size(), 1U);

    const auto& leg = data.getLegs()[0];
    EXPECT_EQ(leg.getAccountId(), _accountId);
    EXPECT_EQ(leg.getInstrumentId(), _instrumentId);
    EXPECT_EQ(leg.getQuantity().toMicroUnits(), 100'000'000LL);
    EXPECT_EQ(leg.getUnitPrice().getAmount(), 150'000'000LL);
    EXPECT_EQ(leg.getUnitPrice().getCurrency(), Currency::USD);
}

TEST_F(
    TransactionRepoFixture,
    AddTransactionTradeEntryAndLegBelongToSameTransaction
)
{
    const auto result = _repo.addTransaction(makeTradeTx());

    const auto txs =
        _repo.getTransactions({_accountId}, finance::TransactionFilter{});

    ASSERT_EQ(txs.size(), 1U);
    EXPECT_EQ(txs[0].getEntries().size(), 1U);

    const auto& data = std::get<finance::TradeData>(txs[0].getData());
    EXPECT_EQ(data.getLegs().size(), 1U);
}

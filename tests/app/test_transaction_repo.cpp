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
#include <variant>
#include <vector>

#include "app/migration/migration_runner.hpp"
#include "app/repos/transaction_repo.hpp"
#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "config/quantity.hpp"
#include "db/database.hpp"
#include "finance/cash.hpp"
#include "finance/trade_data.hpp"
#include "finance/transaction.hpp"
#include "finance/transaction_entry.hpp"
#include "utils/timestamp.hpp"

namespace
{
    std::filesystem::path unique_temp_db_path()
    {
        const auto tmp = std::filesystem::temp_directory_path();

        std::random_device                           rd;
        std::mt19937_64                              gen(rd());
        std::uniform_int_distribution<std::uint64_t> dis;

        return tmp /
               ("molartracker_txrepo_" + std::to_string(dis(gen)) + ".sqlite");
    }

    struct TempDbFile
    {
        std::filesystem::path path;

        explicit TempDbFile(std::filesystem::path p) : path(std::move(p)) {}

        ~TempDbFile()
        {
            std::error_code ec;
            std::filesystem::remove(path, ec);
            std::filesystem::remove(
                std::filesystem::path(path.string() + ".bck"),
                ec
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
        const AccountId    _accountId{1};
        const InstrumentId _instrumentId{1};

        TempDbFile           _tempFile{unique_temp_db_path()};
        db::Database         _db{_tempFile.path};
        app::MigrationRunner _migrationRunner{_db};
        app::TransactionRepo _repo{_db};

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
                    finance::Cash{Currency::USD, amount}}},
                comment};
        }

        [[nodiscard]] finance::Transaction makeTradeTx() const
        {
            finance::TradeData data;
            data.addLeg(finance::TradeLeg{
                _accountId,
                _instrumentId,
                Quantity{100'000'000LL},
                finance::Cash{Currency::USD, 150'000'000LL}});

            return finance::Transaction{
                TransactionId::invalid(),
                Timestamp::fromInt64(TEST_TS),
                TransactionStatus::Completed,
                data,
                {finance::TransactionEntry{
                    TransactionEntryId::invalid(),
                    _accountId,
                    finance::Cash{Currency::USD, -15'000'000'000LL}}},
                "trade comment"};
        }
    };

}   // namespace

// ---------------------------------------------------------------------------
// getTransactions
// ---------------------------------------------------------------------------

TEST_F(TransactionRepoFixture, GetTransactions_EmptyDatabase_ReturnsEmpty)
{
    const auto txs = _repo.getTransactions();

    EXPECT_TRUE(txs.empty());
}

// ---------------------------------------------------------------------------
// addTransaction — returned ID
// ---------------------------------------------------------------------------

TEST_F(TransactionRepoFixture, AddTransaction_Cash_ReturnsPositiveId)
{
    const auto id = _repo.addTransaction(makeCashTx());

    EXPECT_TRUE(id.isValid());
    EXPECT_GT(id.value(), 0);
}

TEST_F(
    TransactionRepoFixture,
    AddTransaction_TwoTransactions_IdsAreDistinctAndIncreasing
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

TEST_F(TransactionRepoFixture, AddTransaction_Cash_SingleTransactionRetrieved)
{
    _repo.addTransaction(makeCashTx());

    const auto txs = _repo.getTransactions();

    ASSERT_EQ(txs.size(), 1u);
}

TEST_F(
    TransactionRepoFixture,
    AddTransaction_Cash_TransactionTypeIsDataTypeCash
)
{
    _repo.addTransaction(makeCashTx());

    const auto txs = _repo.getTransactions();

    ASSERT_EQ(txs.size(), 1u);
    EXPECT_EQ(txs[0].getType(), TransactionDataType::Cash);
}

TEST_F(TransactionRepoFixture, AddTransaction_Cash_StatusIsPreserved)
{
    _repo.addTransaction(makeCashTx());

    const auto txs = _repo.getTransactions();

    ASSERT_EQ(txs.size(), 1u);
    EXPECT_EQ(txs[0].getStatus(), TransactionStatus::Completed);
}

TEST_F(TransactionRepoFixture, AddTransaction_Cash_TimestampIsPreserved)
{
    _repo.addTransaction(makeCashTx());

    const auto txs = _repo.getTransactions();

    ASSERT_EQ(txs.size(), 1u);
    EXPECT_EQ(txs[0].getTimestamp().toInt64(), TEST_TS);
}

TEST_F(TransactionRepoFixture, AddTransaction_Cash_AssignedIdMatchesReturnedId)
{
    const auto insertedId = _repo.addTransaction(makeCashTx());

    const auto txs = _repo.getTransactions();

    ASSERT_EQ(txs.size(), 1u);
    EXPECT_EQ(txs[0].getId(), insertedId);
}

// ---------------------------------------------------------------------------
// addTransaction — comment field
// ---------------------------------------------------------------------------

TEST_F(TransactionRepoFixture, AddTransaction_WithComment_CommentIsPreserved)
{
    _repo.addTransaction(makeCashTx("my note"));

    const auto txs = _repo.getTransactions();

    ASSERT_EQ(txs.size(), 1u);
    ASSERT_TRUE(txs[0].getComment().has_value());
    EXPECT_EQ(*txs[0].getComment(), "my note");
}

TEST_F(TransactionRepoFixture, AddTransaction_WithNullComment_CommentIsAbsent)
{
    _repo.addTransaction(makeCashTx(std::nullopt));

    const auto txs = _repo.getTransactions();

    ASSERT_EQ(txs.size(), 1u);
    EXPECT_FALSE(txs[0].getComment().has_value());
}

// ---------------------------------------------------------------------------
// addTransaction — entries
// ---------------------------------------------------------------------------

TEST_F(
    TransactionRepoFixture,
    AddTransaction_Cash_SingleEntry_EntryIsRetrieved
)
{
    _repo.addTransaction(makeCashTx(std::nullopt, 250'000LL));

    const auto txs = _repo.getTransactions();

    ASSERT_EQ(txs.size(), 1u);
    ASSERT_EQ(txs[0].getEntries().size(), 1u);

    const auto& entry = txs[0].getEntries()[0];
    EXPECT_EQ(entry.getAccountId(), _accountId);
    EXPECT_EQ(entry.getAmount(), 250'000LL);
    EXPECT_EQ(entry.getCurrency(), Currency::USD);
}

TEST_F(
    TransactionRepoFixture,
    AddTransaction_Cash_MultipleEntries_AllEntriesRetrieved
)
{
    finance::Transaction tx{
        TransactionId::invalid(),
        Timestamp::fromInt64(TEST_TS),
        TransactionStatus::Completed,
        finance::CashData{},
        {finance::TransactionEntry{
             TransactionEntryId::invalid(),
             _accountId,
             finance::Cash{Currency::USD, 100'000LL}},
         finance::TransactionEntry{
             TransactionEntryId::invalid(),
             _accountId,
             finance::Cash{Currency::EUR, 200'000LL}}},
        std::nullopt};

    _repo.addTransaction(tx);

    const auto txs = _repo.getTransactions();

    ASSERT_EQ(txs.size(), 1u);
    EXPECT_EQ(txs[0].getEntries().size(), 2u);
}

// ---------------------------------------------------------------------------
// addTransaction — multiple transactions
// ---------------------------------------------------------------------------

TEST_F(
    TransactionRepoFixture,
    AddTransaction_MultipleTransactions_AllAreRetrieved
)
{
    _repo.addTransaction(makeCashTx("first"));
    _repo.addTransaction(makeCashTx("second"));
    _repo.addTransaction(makeCashTx("third"));

    const auto txs = _repo.getTransactions();

    EXPECT_EQ(txs.size(), 3u);
}

// ---------------------------------------------------------------------------
// addTransaction — Trade round-trip
// ---------------------------------------------------------------------------

TEST_F(TransactionRepoFixture, AddTransaction_Trade_TypeIsDataTypeTrade)
{
    _repo.addTransaction(makeTradeTx());

    const auto txs = _repo.getTransactions();

    ASSERT_EQ(txs.size(), 1u);
    EXPECT_EQ(txs[0].getType(), TransactionDataType::Trade);
}

TEST_F(TransactionRepoFixture, AddTransaction_Trade_LegIsRetrieved)
{
    _repo.addTransaction(makeTradeTx());

    const auto txs = _repo.getTransactions();

    ASSERT_EQ(txs.size(), 1u);

    const auto& data = std::get<finance::TradeData>(txs[0].getData());
    ASSERT_EQ(data.getLegs().size(), 1u);

    const auto& leg = data.getLegs()[0];
    EXPECT_EQ(leg.getAccountId(), _accountId);
    EXPECT_EQ(leg.getInstrumentId(), _instrumentId);
    EXPECT_EQ(leg.getQuantity().toMicroUnits(), 100'000'000LL);
    EXPECT_EQ(leg.getUnitPrice().getAmount(), 150'000'000LL);
    EXPECT_EQ(leg.getUnitPrice().getCurrency(), Currency::USD);
}

TEST_F(
    TransactionRepoFixture,
    AddTransaction_Trade_EntryAndLegBelongToSameTransaction
)
{
    _repo.addTransaction(makeTradeTx());

    const auto txs = _repo.getTransactions();

    ASSERT_EQ(txs.size(), 1u);
    EXPECT_EQ(txs[0].getEntries().size(), 1u);

    const auto& data = std::get<finance::TradeData>(txs[0].getData());
    EXPECT_EQ(data.getLegs().size(), 1u);
}

#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <random>
#include <string>

#include "db/database.hpp"
#include "db/statement.hpp"
#include "db/transaction.hpp"

namespace
{
    // ---------- temp db file (portable; fixes Windows ":memory:" path issues)
    // ----------

    std::filesystem::path unique_db_file_path()
    {
        namespace fs = std::filesystem;

        fs::path base;
        try
        {
            base = fs::temp_directory_path();
        }
        catch (...)
        {
            base = fs::current_path();
        }

        std::random_device random;
        const auto         random1 = static_cast<unsigned>(random());
        const auto         random2 = static_cast<unsigned>(random());

        return base / ("molartracker_test_" + std::to_string(random1) + "_" +
                       std::to_string(random2) + ".sqlite");
    }

    class TempDbFile
    {
       public:
        TempDbFile() : _path(unique_db_file_path()) {}

        TempDbFile(TempDbFile const&)                      = delete;
        TempDbFile& operator=(TempDbFile const&)           = delete;
        TempDbFile(TempDbFile&& other) noexcept            = default;
        TempDbFile& operator=(TempDbFile&& other) noexcept = default;

        ~TempDbFile()
        {
            std::error_code errorCode;
            std::filesystem::remove(_path, errorCode);
        }

        const std::filesystem::path& path() const noexcept { return _path; }

       private:
        std::filesystem::path _path;
    };

    db::Database make_test_db(TempDbFile& file)
    {
        return db::Database{file.path()};
    }

    // ---------- schema + query helpers ----------

    void create_schema(std::shared_ptr<db::Database>& db)
    {
        db->execute("CREATE TABLE IF NOT EXISTS t(v INTEGER NOT NULL);");
        db->execute("DELETE FROM t;");
    }

    std::int64_t scalar_int64(
        std::shared_ptr<db::Database>& db,
        const std::string&             sql
    )
    {
        // Adjust if your Database API differs (e.g. prepare_statement()).
        auto stmt = db->prepare(sql);

        const auto resultStep1 = stmt.step();
        EXPECT_EQ(resultStep1, db::StepResult::RowAvailable);

        const auto value = stmt.columnInt64(0);

        const auto resultStep2 = stmt.step();
        EXPECT_EQ(resultStep2, db::StepResult::Done);

        return value;
    }

    std::int64_t count_rows(std::shared_ptr<db::Database>& db)
    {
        return scalar_int64(db, "SELECT COUNT(*) FROM t;");
    }
}   // namespace

TEST(TransactionTest, CommitPersistsChanges)
{
    TempDbFile file;
    auto       _db = make_test_db(file);
    auto       db  = std::make_shared<db::Database>(std::move(_db));

    create_schema(db);

    {
        db::Transaction transaction{db, false};
        EXPECT_TRUE(transaction.isActive());

        db->execute("INSERT INTO t(v) VALUES(1);");
        db->execute("INSERT INTO t(v) VALUES(2);");

        transaction.commit();
        EXPECT_FALSE(transaction.isActive());
    }

    EXPECT_EQ(count_rows(db), 2);
}

TEST(TransactionTest, RollbackDiscardsChanges)
{
    TempDbFile file;
    auto       _db = make_test_db(file);
    auto       db  = std::make_shared<db::Database>(std::move(_db));

    create_schema(db);

    {
        db::Transaction transaction{db, false};
        EXPECT_TRUE(transaction.isActive());

        db->execute("INSERT INTO t(v) VALUES(1);");
        db->execute("INSERT INTO t(v) VALUES(2);");

        transaction.rollback();
        EXPECT_FALSE(transaction.isActive());
    }

    EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, DestructorRollsBackIfStillActive)
{
    TempDbFile file;
    auto       _db = make_test_db(file);
    auto       db  = std::make_shared<db::Database>(std::move(_db));

    create_schema(db);

    {
        db::Transaction transaction{db, false};
        EXPECT_TRUE(transaction.isActive());

        db->execute("INSERT INTO t(v) VALUES(123);");
        // no commit/rollback -> destructor should rollback
    }

    EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, CommitIsIdempotentAfterFirstCommit)
{
    TempDbFile file;
    auto       _db = make_test_db(file);
    auto       db  = std::make_shared<db::Database>(std::move(_db));

    create_schema(db);

    db::Transaction transaction{db, false};
    EXPECT_TRUE(transaction.isActive());

    db->execute("INSERT INTO t(v) VALUES(1);");

    transaction.commit();
    EXPECT_FALSE(transaction.isActive());

    EXPECT_NO_THROW(transaction.commit());     // no-op
    EXPECT_NO_THROW(transaction.rollback());   // no-op

    EXPECT_EQ(count_rows(db), 1);
}

TEST(TransactionTest, RollbackIsIdempotentAfterFirstRollback)
{
    TempDbFile file;
    auto       _db = make_test_db(file);
    auto       db  = std::make_shared<db::Database>(std::move(_db));

    create_schema(db);

    db::Transaction transaction{db, false};
    EXPECT_TRUE(transaction.isActive());

    db->execute("INSERT INTO t(v) VALUES(1);");

    transaction.rollback();
    EXPECT_FALSE(transaction.isActive());

    EXPECT_NO_THROW(transaction.rollback());   // no-op
    EXPECT_NO_THROW(transaction.commit());     // no-op

    EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, MoveConstructorTransfersActivityAndDisarmsSource)
{
    TempDbFile file;
    auto       _db = make_test_db(file);
    auto       db  = std::make_shared<db::Database>(std::move(_db));

    create_schema(db);

    db::Transaction transactionA{db, false};
    EXPECT_TRUE(transactionA.isActive());

    db->execute("INSERT INTO t(v) VALUES(7);");

    db::Transaction transactionB{std::move(transactionA)};
    EXPECT_TRUE(transactionB.isActive());
    EXPECT_FALSE(transactionA.isActive());

    transactionB.commit();
    EXPECT_FALSE(transactionB.isActive());

    EXPECT_EQ(count_rows(db), 1);
}

TEST(TransactionTest, MoveAssignmentTransfersActivityAndDisarmsSource)
{
    TempDbFile file;
    auto       _db = make_test_db(file);
    auto       db  = std::make_shared<db::Database>(std::move(_db));

    create_schema(db);

    // b must be inactive before move-assign (and must not start while a is
    // active)
    db::Transaction transactionB{db, false};
    transactionB.rollback();
    EXPECT_FALSE(transactionB.isActive());

    db::Transaction transactionA{db, false};
    EXPECT_TRUE(transactionA.isActive());

    db->execute("INSERT INTO t(v) VALUES(9);");

    transactionB = std::move(transactionA);

    EXPECT_TRUE(transactionB.isActive());
    EXPECT_FALSE(transactionA.isActive());

    transactionB.rollback();
    EXPECT_FALSE(transactionB.isActive());

    EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, ImmediateTransactionBehavesLikeTransaction)
{
    TempDbFile file;
    auto       _db = make_test_db(file);
    auto       db  = std::make_shared<db::Database>(std::move(_db));

    create_schema(db);

    {
        db::Transaction transaction{db, true};   // BEGIN IMMEDIATE
        EXPECT_TRUE(transaction.isActive());

        db->execute("INSERT INTO t(v) VALUES(1);");

        transaction.rollback();
        EXPECT_FALSE(transaction.isActive());
    }

    EXPECT_EQ(count_rows(db), 0);
}
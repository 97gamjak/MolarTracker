#include <gtest/gtest.h>

#include <string>

#include "db/database.hpp"
#include "db/statement.hpp"
#include "db/transaction.hpp"
#include "test_fixtures.hpp"

namespace
{
    db::Database make_test_db(tests::TempDbFile& file)
    {
        return db::Database{file.path()};
    }

    // ---------- schema + query helpers ----------

    void create_schema(db::Database& db)
    {
        static_cast<void>(
            db.execute("CREATE TABLE IF NOT EXISTS t(v INTEGER NOT NULL);")
        );
        static_cast<void>(db.execute("DELETE FROM t;"));
    }

    std::int64_t scalar_int64(db::Database& db, const std::string& sql)
    {
        // Adjust if your Database API differs (e.g. prepare_statement()).
        auto stmt = db.prepare(sql);

        const auto resultStep1 = stmt.step();
        EXPECT_EQ(resultStep1, db::StepResult::RowAvailable);

        const auto value = stmt.columnInt64(0);

        const auto resultStep2 = stmt.step();
        EXPECT_EQ(resultStep2, db::StepResult::Done);

        return value;
    }

    std::int64_t count_rows(db::Database& db)
    {
        return scalar_int64(db, "SELECT COUNT(*) FROM t;");
    }
}   // namespace

TEST(TransactionTest, CommitPersistsChanges)
{
    tests::TempDbFile file;
    auto              db = make_test_db(file);

    create_schema(db);

    {
        db::Transaction transaction{db, false};
        EXPECT_TRUE(transaction.isActive());

        static_cast<void>(db.execute("INSERT INTO t(v) VALUES(1);"));
        static_cast<void>(db.execute("INSERT INTO t(v) VALUES(2);"));

        static_cast<void>(transaction.commit());
        EXPECT_FALSE(transaction.isActive());
    }

    EXPECT_EQ(count_rows(db), 2);
}

TEST(TransactionTest, RollbackDiscardsChanges)
{
    tests::TempDbFile file;
    auto              db = make_test_db(file);

    create_schema(db);

    {
        db::Transaction transaction{db, false};
        EXPECT_TRUE(transaction.isActive());

        static_cast<void>(db.execute("INSERT INTO t(v) VALUES(1);"));
        static_cast<void>(db.execute("INSERT INTO t(v) VALUES(2);"));

        static_cast<void>(transaction.rollback());
        EXPECT_FALSE(transaction.isActive());
    }

    EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, DestructorRollsBackIfStillActive)
{
    tests::TempDbFile file;
    auto              db = make_test_db(file);

    create_schema(db);

    {
        db::Transaction transaction{db, false};
        EXPECT_TRUE(transaction.isActive());

        static_cast<void>(db.execute("INSERT INTO t(v) VALUES(123);"));
        // no commit/rollback -> destructor should rollback
    }

    EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, CommitIsIdempotentAfterFirstCommit)
{
    tests::TempDbFile file;
    auto              db = make_test_db(file);

    create_schema(db);

    db::Transaction transaction{db, false};
    EXPECT_TRUE(transaction.isActive());

    static_cast<void>(db.execute("INSERT INTO t(v) VALUES(1);"));

    static_cast<void>(transaction.commit());
    EXPECT_FALSE(transaction.isActive());

    EXPECT_TRUE(transaction.commit().has_value());     // no-op
    EXPECT_TRUE(transaction.rollback().has_value());   // no-op

    EXPECT_EQ(count_rows(db), 1);
}

TEST(TransactionTest, RollbackIsIdempotentAfterFirstRollback)
{
    tests::TempDbFile file;
    auto              db = make_test_db(file);

    create_schema(db);

    db::Transaction transaction{db, false};
    EXPECT_TRUE(transaction.isActive());

    static_cast<void>(db.execute("INSERT INTO t(v) VALUES(1);"));

    static_cast<void>(transaction.rollback());
    EXPECT_FALSE(transaction.isActive());

    EXPECT_TRUE(transaction.rollback().has_value());   // no-op
    EXPECT_TRUE(transaction.commit().has_value());     // no-op

    EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, MoveConstructorTransfersActivityAndDisarmsSource)
{
    tests::TempDbFile file;
    auto              db = make_test_db(file);

    create_schema(db);

    db::Transaction transactionA{db, false};
    EXPECT_TRUE(transactionA.isActive());

    static_cast<void>(db.execute("INSERT INTO t(v) VALUES(7);"));

    db::Transaction transactionB{std::move(transactionA)};
    EXPECT_TRUE(transactionB.isActive());
    EXPECT_FALSE(transactionA.isActive());

    static_cast<void>(transactionB.commit());
    EXPECT_FALSE(transactionB.isActive());

    EXPECT_EQ(count_rows(db), 1);
}

TEST(TransactionTest, MoveAssignmentTransfersActivityAndDisarmsSource)
{
    tests::TempDbFile file;
    auto              db = make_test_db(file);

    create_schema(db);

    // b must be inactive before move-assign (and must not start while a is
    // active)
    db::Transaction transactionB{db, false};
    static_cast<void>(transactionB.rollback());
    EXPECT_FALSE(transactionB.isActive());

    db::Transaction transactionA{db, false};
    EXPECT_TRUE(transactionA.isActive());

    static_cast<void>(db.execute("INSERT INTO t(v) VALUES(9);"));

    transactionB = std::move(transactionA);

    EXPECT_TRUE(transactionB.isActive());
    EXPECT_FALSE(transactionA.isActive());

    static_cast<void>(transactionB.rollback());
    EXPECT_FALSE(transactionB.isActive());

    EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, ImmediateTransactionBehavesLikeTransaction)
{
    tests::TempDbFile file;
    auto              db = make_test_db(file);

    create_schema(db);

    {
        db::Transaction transaction{db, true};   // BEGIN IMMEDIATE
        EXPECT_TRUE(transaction.isActive());

        static_cast<void>(db.execute("INSERT INTO t(v) VALUES(1);"));

        static_cast<void>(transaction.rollback());
        EXPECT_FALSE(transaction.isActive());
    }

    EXPECT_EQ(count_rows(db), 0);
}

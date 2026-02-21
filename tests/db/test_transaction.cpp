#include <gtest/gtest.h>

#include <cstdint>
#include <string>

#include "db/database.hpp"
#include "db/statement.hpp"
#include "db/transaction.hpp"

namespace
{
    db::Database make_in_memory_db() { return db::Database{":memory:"}; }

    void create_schema(db::Database& db)
    {
        db.execute("CREATE TABLE IF NOT EXISTS t(v INTEGER NOT NULL);");
        db.execute("DELETE FROM t;");
    }

    std::int64_t scalar_int64(db::Database& db, const std::string& sql)
    {
        // Adjust if your API differs (e.g. db.prepare_statement / db.prepare).
        auto st = db.prepare(sql);

        const auto r1 = st.step();
        EXPECT_EQ(r1, db::StepResult::RowAvailable);

        const auto value = st.columnInt64(0);

        const auto r2 = st.step();
        EXPECT_EQ(r2, db::StepResult::Done);

        return value;
    }

    std::int64_t count_rows(db::Database& db)
    {
        return scalar_int64(db, "SELECT COUNT(*) FROM t;");
    }
}   // namespace

TEST(TransactionTest, CommitPersistsChanges)
{
    auto db = make_in_memory_db();
    create_schema(db);

    {
        db::Transaction tx{db, false};
        EXPECT_TRUE(tx.isActive());

        db.execute("INSERT INTO t(v) VALUES(1);");
        db.execute("INSERT INTO t(v) VALUES(2);");

        tx.commit();
        EXPECT_FALSE(tx.isActive());
    }

    EXPECT_EQ(count_rows(db), 2);
}

TEST(TransactionTest, RollbackDiscardsChanges)
{
    auto db = make_in_memory_db();
    create_schema(db);

    {
        db::Transaction tx{db, false};
        EXPECT_TRUE(tx.isActive());

        db.execute("INSERT INTO t(v) VALUES(1);");
        db.execute("INSERT INTO t(v) VALUES(2);");

        tx.rollback();
        EXPECT_FALSE(tx.isActive());
    }

    EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, DestructorRollsBackIfStillActive)
{
    auto db = make_in_memory_db();
    create_schema(db);

    {
        db::Transaction tx{db, false};
        db.execute("INSERT INTO t(v) VALUES(123);");
        // no commit/rollback -> destructor should rollback
    }

    EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, CommitIsIdempotentAfterFirstCommit)
{
    auto db = make_in_memory_db();
    create_schema(db);

    db::Transaction tx{db, false};
    db.execute("INSERT INTO t(v) VALUES(1);");

    EXPECT_TRUE(tx.isActive());
    tx.commit();
    EXPECT_FALSE(tx.isActive());

    EXPECT_NO_THROW(tx.commit());     // no-op
    EXPECT_NO_THROW(tx.rollback());   // no-op

    EXPECT_EQ(count_rows(db), 1);
}

TEST(TransactionTest, RollbackIsIdempotentAfterFirstRollback)
{
    auto db = make_in_memory_db();
    create_schema(db);

    db::Transaction tx{db, false};
    db.execute("INSERT INTO t(v) VALUES(1);");

    EXPECT_TRUE(tx.isActive());
    tx.rollback();
    EXPECT_FALSE(tx.isActive());

    EXPECT_NO_THROW(tx.rollback());   // no-op
    EXPECT_NO_THROW(tx.commit());     // no-op

    EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, MoveConstructorTransfersActivityAndDisarmsSource)
{
    auto db = make_in_memory_db();
    create_schema(db);

    db::Transaction a{db, false};
    EXPECT_TRUE(a.isActive());

    db.execute("INSERT INTO t(v) VALUES(7);");

    db::Transaction b{std::move(a)};
    EXPECT_TRUE(b.isActive());
    EXPECT_FALSE(a.isActive());

    b.commit();
    EXPECT_FALSE(b.isActive());

    EXPECT_EQ(count_rows(db), 1);
}

TEST(TransactionTest, MoveAssignmentTransfersActivityAndDisarmsSource)
{
    auto db = make_in_memory_db();
    create_schema(db);

    // b must be inactive before move-assign (and must not start while a is
    // active)
    db::Transaction b{db, false};
    b.rollback();
    EXPECT_FALSE(b.isActive());

    db::Transaction a{db, false};
    EXPECT_TRUE(a.isActive());

    db.execute("INSERT INTO t(v) VALUES(9);");

    b = std::move(a);

    EXPECT_TRUE(b.isActive());
    EXPECT_FALSE(a.isActive());

    b.rollback();
    EXPECT_FALSE(b.isActive());

    EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, ImmediateTransactionBehavesLikeTransaction)
{
    auto db = make_in_memory_db();
    create_schema(db);

    {
        db::Transaction tx{db, true};   // BEGIN IMMEDIATE
        EXPECT_TRUE(tx.isActive());
        db.execute("INSERT INTO t(v) VALUES(1);");
        tx.rollback();
    }

    EXPECT_EQ(count_rows(db), 0);
}
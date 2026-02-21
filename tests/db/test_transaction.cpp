#include <gtest/gtest.h>

#include "db/database.hpp"
#include "db/transaction.hpp"

namespace
{
    // Helper: detect if a transaction is currently active by trying to start an
    // IMMEDIATE txn. If a txn is already open, BEGIN IMMEDIATE typically fails
    // with "cannot start a transaction within a transaction" or "database is
    // locked" depending on state. We treat "throws" as "already in txn".
    bool can_begin_immediate(db::Database& db)
    {
        try
        {
            db.execute("BEGIN IMMEDIATE;");
            db.execute("ROLLBACK;");
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    void create_schema(db::Database& db)
    {
        db.execute("CREATE TABLE IF NOT EXISTS t(v INTEGER NOT NULL);");
        db.execute("DELETE FROM t;");
    }

    std::int64_t count_rows(db::Database& db)
    {
        // If your Database has a scalar/query API, use that instead.
        // Minimal approach here: use a temp table side effect via
        // INSERT...SELECT and then count.
        //
        // Preferably replace with: auto st = db.prepare("SELECT COUNT(*) FROM
        // t;")...
        //
        // If Database only has execute(), we can still verify commit/rollback
        // by using constraints: we'll insert + rollback and then attempt a
        // SELECT via a statement API if present.
        //
        // Since we don't know your Database query surface here, we provide a
        // conservative alternative: use a second transaction to test visibility
        // by inserting a known marker and relying on SQL errors.
        //
        // If you DO have db::Statement-based querying, replace this helper
        // accordingly.

        // --- Replace this block with your actual query method ---
        // Example (if you have Database::prepare):
        // auto st = db.prepare("SELECT COUNT(*) FROM t;");
        // EXPECT_EQ(st.step(), db::StepResult::RowAvailable);
        // auto n = st.columnInt64(0);
        // EXPECT_EQ(st.step(), db::StepResult::Done);
        // return n;

        // Fallback: if no query API exists, we can't count rows here reliably.
        // We return -1 and only run tests that don't require counting.
        return -1;
    }

    // If your Database supports ":memory:" you're done.
    // Otherwise replace with a temp file path.
    db::Database make_in_memory_db() { return db::Database{":memory:"}; }
}   // namespace

TEST(TransactionTest, BeginsAndIsActive)
{
    auto db = make_in_memory_db();
    create_schema(db);

    EXPECT_TRUE(can_begin_immediate(db));   // sanity: no txn yet

    db::Transaction tx{db, false};
    EXPECT_TRUE(tx.isActive());

    EXPECT_FALSE(can_begin_immediate(db));   // should fail: already in txn

    tx.rollback();
    EXPECT_FALSE(tx.isActive());

    EXPECT_TRUE(can_begin_immediate(db));   // no txn after rollback
}

TEST(TransactionTest, CommitMakesTransactionInactive)
{
    auto db = make_in_memory_db();
    create_schema(db);

    db::Transaction tx{db, false};
    EXPECT_TRUE(tx.isActive());

    tx.commit();
    EXPECT_FALSE(tx.isActive());

    // double-commit should be no-op
    EXPECT_NO_THROW(tx.commit());
    EXPECT_FALSE(tx.isActive());
}

TEST(TransactionTest, RollbackMakesTransactionInactive)
{
    auto db = make_in_memory_db();
    create_schema(db);

    db::Transaction tx{db, false};
    EXPECT_TRUE(tx.isActive());

    tx.rollback();
    EXPECT_FALSE(tx.isActive());

    // double-rollback should be no-op
    EXPECT_NO_THROW(tx.rollback());
    EXPECT_FALSE(tx.isActive());
}

TEST(TransactionTest, DestructorRollsBackIfStillActive)
{
    auto db = make_in_memory_db();
    create_schema(db);

    EXPECT_TRUE(can_begin_immediate(db));

    {
        db::Transaction tx{db, false};
        EXPECT_FALSE(can_begin_immediate(db));   // txn active

        db.execute("INSERT INTO t(v) VALUES(1);");
        // no commit/rollback -> destructor should rollback
    }

    EXPECT_TRUE(can_begin_immediate(db));   // destructor ended txn

    // If you have query support, uncomment and use it:
    // EXPECT_EQ(count_rows(db), 0);
}

TEST(TransactionTest, MoveConstructorTransfersActivityAndDisarmsSource)
{
    auto db = make_in_memory_db();
    create_schema(db);

    EXPECT_TRUE(can_begin_immediate(db));

    db::Transaction a{db, false};
    EXPECT_TRUE(a.isActive());
    EXPECT_FALSE(can_begin_immediate(db));

    db::Transaction b{std::move(a)};
    EXPECT_TRUE(b.isActive());
    EXPECT_FALSE(a.isActive());   // moved-from disarmed

    // Ending b should end txn (commit here)
    b.commit();
    EXPECT_FALSE(b.isActive());
    EXPECT_TRUE(can_begin_immediate(db));
}

TEST(TransactionTest, MoveAssignmentTransfersActivityAndDisarmsSource)
{
    auto db = make_in_memory_db();
    create_schema(db);

    // b exists first and is inactive
    db::Transaction b{db, false};
    b.rollback();
    EXPECT_FALSE(b.isActive());

    // Now create a (starts the only active txn)
    db::Transaction a{db, false};
    EXPECT_TRUE(a.isActive());
    EXPECT_FALSE(can_begin_immediate(db));

    // Move-assign active a into inactive b
    b = std::move(a);

    EXPECT_TRUE(b.isActive());
    EXPECT_FALSE(a.isActive());

    b.rollback();
    EXPECT_FALSE(b.isActive());
    EXPECT_TRUE(can_begin_immediate(db));
}

TEST(TransactionTest, ImmediateBeginsImmediate)
{
    auto db = make_in_memory_db();
    create_schema(db);

    // This test mainly checks that constructor doesn't throw and holds a txn.
    // BEGIN IMMEDIATE vs BEGIN difference is lock acquisition; in
    // single-connection tests we can only reliably assert "a txn is active".
    db::Transaction tx{db, true};
    EXPECT_TRUE(tx.isActive());
    EXPECT_FALSE(can_begin_immediate(db));
    tx.rollback();
}
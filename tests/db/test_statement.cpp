#include "test_statement.hpp"

#include <string>

#include "db/db_exception.hpp"

namespace db::test
{
    TEST_F(StatementFixture, IsValidAfterConstruction)
    {
        Prepared prep{db.native(), "SELECT 1;"};
        auto     stmt = make_statement(db.native(), prep, "SELECT 1;");
        EXPECT_TRUE(stmt.is_valid());
    }

    TEST_F(StatementFixture, MoveConstructorTransfersOwnership)
    {
        Prepared prep{db.native(), "SELECT 1;"};

        db::Statement a = make_statement(db.native(), prep, "SELECT 1;");
        db::Statement b{std::move(a)};

        EXPECT_TRUE(b.is_valid());
        EXPECT_FALSE(a.is_valid());
    }

    TEST_F(StatementFixture, MoveAssignmentTransfersOwnership)
    {
        Prepared prep1{db.native(), "SELECT 1;"};
        Prepared prep2{db.native(), "SELECT 2;"};

        db::Statement a = make_statement(db.native(), prep1, "SELECT 1;");
        db::Statement b = make_statement(db.native(), prep2, "SELECT 2;");

        b = std::move(a);

        EXPECT_TRUE(b.is_valid());
        EXPECT_FALSE(a.is_valid());
    }

    TEST_F(StatementFixture, StepReturnsRowAndDone)
    {
        Prepared prep{db.native(), "SELECT 1;"};
        auto     stmt = make_statement(db.native(), prep, "SELECT 1;");

        EXPECT_EQ(stmt.step(), db::StepResult::RowAvailable);
        EXPECT_EQ(stmt.step(), db::StepResult::Done);
    }

    TEST_F(StatementFixture, ExecuteToCompletionThrowsIfRowProduced)
    {
        Prepared prep{db.native(), "SELECT 1;"};
        auto     stmt = make_statement(db.native(), prep, "SELECT 1;");

        EXPECT_THROW(stmt.executeToCompletion(), db::SqliteError);
    }

    TEST_F(StatementFixture, ExecuteToCompletionWorksForDDL)
    {
        Prepared prep{db.native(), "CREATE TABLE t(x INTEGER);"};
        auto     stmt =
            make_statement(db.native(), prep, "CREATE TABLE t(x INTEGER);");

        EXPECT_NO_THROW(stmt.executeToCompletion());
    }

    TEST_F(StatementFixture, BindAndReadRoundtrip)
    {
        db.exec("CREATE TABLE items(i INTEGER, d REAL, t TEXT);");

        {
            Prepared prep{
                db.native(),
                "INSERT INTO items(i,d,t) VALUES(?,?,?);"
            };

            auto stmt = make_statement(
                db.native(),
                prep,
                "INSERT INTO items(i,d,t) VALUES(?,?,?);"
            );

            stmt.bindInt64(1, 42);
            stmt.bindDouble(2, 3.5);
            stmt.bindText(3, "hello");

            stmt.executeToCompletion();
        }

        {
            Prepared prep{db.native(), "SELECT i,d,t FROM items;"};

            auto stmt =
                make_statement(db.native(), prep, "SELECT i,d,t FROM items;");

            ASSERT_EQ(stmt.step(), db::StepResult::RowAvailable);

            EXPECT_EQ(stmt.columnInt64(0), 42);
            EXPECT_DOUBLE_EQ(stmt.columnDouble(1), 3.5);
            EXPECT_FALSE(stmt.columnIsNull(2));
            EXPECT_EQ(stmt.columnText(2), "hello");

            EXPECT_EQ(stmt.step(), db::StepResult::Done);
        }
    }

    TEST_F(StatementFixture, BindNullAndColumnIsNullAndColumnTextEmpty)
    {
        db.exec("CREATE TABLE items(i INTEGER, t TEXT);");

        {
            Prepared prep{db.native(), "INSERT INTO items(i,t) VALUES(?,?);"};

            auto stmt = make_statement(
                db.native(),
                prep,
                "INSERT INTO items(i,t) VALUES(?,?);"
            );

            stmt.bindInt64(1, 1);
            stmt.bindNull(2);
            stmt.executeToCompletion();
        }

        {
            Prepared prep{db.native(), "SELECT i,t FROM items;"};
            auto     stmt =
                make_statement(db.native(), prep, "SELECT i,t FROM items;");

            ASSERT_EQ(stmt.step(), db::StepResult::RowAvailable);

            EXPECT_EQ(stmt.columnInt64(0), 1);
            EXPECT_TRUE(stmt.columnIsNull(1));
            EXPECT_TRUE(stmt.columnText(1).empty()
            );   // per your implementation

            EXPECT_EQ(stmt.step(), db::StepResult::Done);
        }
    }

    TEST_F(StatementFixture, ResetAllowsReexecutionAndClearsBindings)
    {
        db.exec("CREATE TABLE t(x INTEGER);");
        db.exec("INSERT INTO t VALUES (1),(2),(3);");

        Prepared prep{db.native(), "SELECT x FROM t WHERE x > ? ORDER BY x;"};
        auto     stmt = make_statement(
            db.native(),
            prep,
            "SELECT x FROM t WHERE x > ? ORDER BY x;"
        );

        stmt.bindInt64(1, 1);

        ASSERT_EQ(stmt.step(), db::StepResult::RowAvailable);
        EXPECT_EQ(stmt.columnInt64(0), 2);

        ASSERT_EQ(stmt.step(), db::StepResult::RowAvailable);
        EXPECT_EQ(stmt.columnInt64(0), 3);

        ASSERT_EQ(stmt.step(), db::StepResult::Done);

        stmt.reset();
        stmt.bindInt64(1, 2);

        ASSERT_EQ(stmt.step(), db::StepResult::RowAvailable);
        EXPECT_EQ(stmt.columnInt64(0), 3);
        ASSERT_EQ(stmt.step(), db::StepResult::Done);

        // reset and do NOT bind => cleared binding => x > NULL yields no rows
        stmt.reset();
        EXPECT_EQ(stmt.step(), db::StepResult::Done);
    }

    TEST(StatementStandaloneTest, InvalidHandleThrows)
    {
        db::Statement stmt{nullptr, nullptr, "SELECT 1;"};

        EXPECT_FALSE(stmt.is_valid());
        EXPECT_THROW((void) stmt.step(), db::SqliteError);
        EXPECT_THROW(stmt.reset(), db::SqliteError);
        EXPECT_THROW(stmt.bindInt64(1, 1), db::SqliteError);
        EXPECT_THROW((void) stmt.columnInt64(0), db::SqliteError);
        EXPECT_THROW((void) stmt.columnDouble(0), db::SqliteError);
        EXPECT_THROW((void) stmt.columnText(0), db::SqliteError);
        EXPECT_THROW((void) stmt.columnIsNull(0), db::SqliteError);
    }

    TEST_F(StatementFixture, BindErrorIncludesSqlText)
    {
        Prepared prep{db.native(), "SELECT ?;"};
        auto     stmt = make_statement(db.native(), prep, "SELECT ?;");

        try
        {
            stmt.bindInt64(0, 1);   // invalid (1-based indices)
            FAIL() << "Expected db::SqliteError";
        }
        catch (db::SqliteError const& e)
        {
            const std::string msg = e.what();
            EXPECT_NE(msg.find("bindInt64"), std::string::npos);
            EXPECT_NE(msg.find("sql: SELECT ?;"), std::string::npos);
        }
    }

    TEST_F(StatementFixture, StepThrowsAfterSchemaChange)
    {
        db.exec("CREATE TABLE s(x INTEGER);");
        db.exec("INSERT INTO s(x) VALUES(1);");

        Prepared prep{db.native(), "SELECT x FROM s;"};
        auto     stmt = make_statement(db.native(), prep, "SELECT x FROM s;");

        db.exec("DROP TABLE s;");

        EXPECT_THROW((void) stmt.step(), db::SqliteError);
    }
}   // namespace db::test
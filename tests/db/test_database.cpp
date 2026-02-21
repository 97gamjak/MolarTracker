// database_tests.cpp
//
// GoogleTest-based tests for db::Database.
//
// Coverage:
//  - open/close/isOpen/nativeHandle
//  - constructor relative path behavior (via CWD change)
//  - execute success + error paths (message contains SQL)
//  - prepare success + error paths (message contains SQL)
//  - lastInsertRowid + changes
//  - move ctor / move assignment semantics
//  - foreign key enforcement toggling
//  - busy timeout behavior under lock contention
//  - open invalid path (directory)
//
// These tests use real SQLite database files under the OS temp directory.

#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <random>
#include <string>
#include <string_view>
#include <utility>

#include "db/database.hpp"
#include "db/db_exception.hpp"
#include "db/statement.hpp"

namespace
{
    std::filesystem::path unique_temp_db_path(
        std::string_view stem = "molartracker_db_test"
    )
    {
        const auto tmp = std::filesystem::temp_directory_path();

        std::random_device                           rd;
        std::mt19937_64                              gen(rd());
        std::uniform_int_distribution<std::uint64_t> dis;

        const auto r = dis(gen);

        return tmp / (std::string(stem) + "_" + std::to_string(r) + ".sqlite");
    }

    struct TempDbFile
    {
        std::filesystem::path path;

        explicit TempDbFile(std::filesystem::path p) : path(std::move(p)) {}

        ~TempDbFile()
        {
            std::error_code ec;
            std::filesystem::remove(path, ec);
        }

        TempDbFile(TempDbFile const&)            = delete;
        TempDbFile& operator=(TempDbFile const&) = delete;
        TempDbFile(TempDbFile&&)                 = delete;
        TempDbFile& operator=(TempDbFile&&)      = delete;
    };

    void expect_contains(std::string const& haystack, std::string_view needle)
    {
        EXPECT_NE(haystack.find(std::string(needle)), std::string::npos)
            << haystack;
    }

    void expect_sqlite_error_contains(
        db::SqliteError const& ex,
        std::string_view       needle1,
        std::string_view       needle2 = {}
    )
    {
        const std::string msg = ex.what();
        expect_contains(msg, needle1);
        if (!needle2.empty())
            expect_contains(msg, needle2);
    }
}   // namespace

TEST(Database, ConstructsAndOpensCreatesFileIsOpenNativeHandleNonNull)
{
    const auto p = unique_temp_db_path();
    TempDbFile cleanup{p};

    EXPECT_FALSE(std::filesystem::exists(p));

    db::Database db(p);
    EXPECT_TRUE(db.isOpen());
    EXPECT_NE(db.nativeHandle(), nullptr);

    EXPECT_TRUE(std::filesystem::exists(p));
}

TEST(Database, ConstructorWithRelativePathCreatesFileAtAbsoluteResolvedLocation)
{
    const auto tmp_dir = std::filesystem::temp_directory_path() /
                         "molartracker_db_relpath_test_dir";
    std::error_code ec;
    std::filesystem::create_directories(tmp_dir, ec);

    const auto old_cwd = std::filesystem::current_path();
    std::filesystem::current_path(tmp_dir);

    const std::filesystem::path rel = "relative_test.sqlite";
    const std::filesystem::path abs = std::filesystem::absolute(rel);

    TempDbFile cleanup{abs};

    EXPECT_FALSE(std::filesystem::exists(abs));

    {
        db::Database db(rel);
        EXPECT_TRUE(db.isOpen());
    }

    EXPECT_TRUE(std::filesystem::exists(abs));

    std::filesystem::current_path(old_cwd);
    std::filesystem::remove_all(tmp_dir, ec);
}

TEST(Database, CloseIsIdempotentAndClearsOpenState)
{
    const auto p = unique_temp_db_path();
    TempDbFile cleanup{p};

    db::Database db(p);
    EXPECT_TRUE(db.isOpen());

    db.close();
    EXPECT_FALSE(db.isOpen());
    EXPECT_EQ(db.nativeHandle(), nullptr);

    EXPECT_NO_THROW(db.close());
    EXPECT_FALSE(db.isOpen());
    EXPECT_EQ(db.nativeHandle(), nullptr);
}

TEST(Database, ExecuteThrowsWhenDatabaseNotOpen)
{
    const auto p = unique_temp_db_path();
    TempDbFile cleanup{p};

    db::Database db(p);
    db.close();

    try
    {
        db.execute("CREATE TABLE t(x INTEGER);");
        FAIL() << "Expected db::SqliteError";
    }
    catch (const db::SqliteError& ex)
    {
        expect_sqlite_error_contains(ex, "Database is not open");
    }
}

TEST(Database, PrepareThrowsWhenDatabaseNotOpen)
{
    const auto p = unique_temp_db_path();
    TempDbFile cleanup{p};

    db::Database db(p);
    db.close();

    EXPECT_THROW((void) db.prepare("SELECT 1;"), db::SqliteError);
}

TEST(Database, ExecuteValidSqlCreatesTableAndInsertsAndChangesReflectsRows)
{
    const auto p = unique_temp_db_path();
    TempDbFile cleanup{p};

    db::Database db(p);

    EXPECT_NO_THROW(db.execute(
        "CREATE TABLE test_items(id INTEGER PRIMARY KEY AUTOINCREMENT, v TEXT);"
    ));

    EXPECT_NO_THROW(db.execute("INSERT INTO test_items(v) VALUES('a');"));
    EXPECT_EQ(db.changes(), 1);
    const auto id1 = db.lastInsertRowid();
    EXPECT_GE(id1, 1);

    EXPECT_NO_THROW(db.execute("INSERT INTO test_items(v) VALUES('b');"));
    EXPECT_EQ(db.changes(), 1);
    const auto id2 = db.lastInsertRowid();
    EXPECT_EQ(id2, id1 + 1);

    EXPECT_NO_THROW(db.execute("UPDATE test_items SET v='c' WHERE id=1;"));
    EXPECT_EQ(db.changes(), 1);

    EXPECT_NO_THROW(db.execute("UPDATE test_items SET v='c' WHERE id=999999;"));
    EXPECT_EQ(db.changes(), 0);
}

TEST(Database, ExecuteInvalidSqlThrowsAndMessageContainsSql)
{
    const auto p = unique_temp_db_path();
    TempDbFile cleanup{p};

    db::Database db(p);

    const std::string bad_sql = "CREAT TABLE nope(x);";

    try
    {
        db.execute(bad_sql);
        FAIL() << "Expected db::SqliteError";
    }
    catch (const db::SqliteError& ex)
    {
        expect_sqlite_error_contains(ex, "sqlite execute failed", bad_sql);
    }
}

TEST(Database, PrepareValidSqlDoesNotThrow)
{
    const auto p = unique_temp_db_path();
    TempDbFile cleanup{p};

    db::Database db(p);
    db.execute("CREATE TABLE t(id INTEGER PRIMARY KEY, v INTEGER);");
    db.execute("INSERT INTO t(id, v) VALUES(1, 42);");

    EXPECT_NO_THROW((void) db.prepare("SELECT v FROM t WHERE id=1;"));
}

TEST(Database, PrepareInvalidSqlThrowsAndMessageContainsSql)
{
    const auto p = unique_temp_db_path();
    TempDbFile cleanup{p};

    db::Database db(p);

    const std::string bad_sql = "SELECT FROM;";

    try
    {
        (void) db.prepare(bad_sql);
        FAIL() << "Expected db::SqliteError";
    }
    catch (const db::SqliteError& ex)
    {
        expect_sqlite_error_contains(ex, "sqlite prepare failed", bad_sql);
    }
}

TEST(Database, EnableForeignKeysEnforcesConstraintsWhenOnAndAllowsWhenOff)
{
    const auto p = unique_temp_db_path();
    TempDbFile cleanup{p};

    db::Database db(p);

    db.execute("CREATE TABLE parent(id INTEGER PRIMARY KEY);");
    db.execute(
        "CREATE TABLE child(id INTEGER PRIMARY KEY, parent_id INTEGER, "
        "FOREIGN KEY(parent_id) REFERENCES parent(id));"
    );

    EXPECT_THROW(
        db.execute("INSERT INTO child(id, parent_id) VALUES(1, 999);"),
        db::SqliteError
    );

    EXPECT_NO_THROW(db.enableForeignKeys(false));
    EXPECT_NO_THROW(
        db.execute("INSERT INTO child(id, parent_id) VALUES(2, 999);")
    );
    EXPECT_EQ(db.changes(), 1);

    EXPECT_NO_THROW(db.enableForeignKeys(true));
    EXPECT_THROW(
        db.execute("INSERT INTO child(id, parent_id) VALUES(3, 12345);"),
        db::SqliteError
    );
}

TEST(Database, MoveConstructorTransfersOwnershipAndLeavesOtherClosed)
{
    const auto p = unique_temp_db_path();
    TempDbFile cleanup{p};

    db::Database a(p);
    ASSERT_TRUE(a.isOpen());
    ASSERT_NE(a.nativeHandle(), nullptr);

    sqlite3* const handle_a = a.nativeHandle();

    db::Database b(std::move(a));

    EXPECT_FALSE(a.isOpen());
    EXPECT_EQ(a.nativeHandle(), nullptr);

    EXPECT_TRUE(b.isOpen());
    EXPECT_EQ(b.nativeHandle(), handle_a);

    EXPECT_NO_THROW(b.execute("CREATE TABLE t(x INTEGER);"));
}

TEST(Database, MoveAssignmentClosesTargetThenTransfersOwnership)
{
    const auto p1 = unique_temp_db_path("db_test_a");
    const auto p2 = unique_temp_db_path("db_test_b");
    TempDbFile cleanup1{p1};
    TempDbFile cleanup2{p2};

    db::Database a(p1);
    db::Database b(p2);

    sqlite3* const handle_a = a.nativeHandle();
    sqlite3* const handle_b = b.nativeHandle();
    ASSERT_NE(handle_a, nullptr);
    ASSERT_NE(handle_b, nullptr);
    ASSERT_NE(handle_a, handle_b);

    b = std::move(a);

    EXPECT_FALSE(a.isOpen());
    EXPECT_EQ(a.nativeHandle(), nullptr);

    EXPECT_TRUE(b.isOpen());
    EXPECT_EQ(b.nativeHandle(), handle_a);

    EXPECT_NO_THROW(b.execute("CREATE TABLE t(x INTEGER);"));
}

TEST(Database, BusyTimeoutUnderWriteLockEventuallyThrows)
{
    const auto p = unique_temp_db_path();
    TempDbFile cleanup{p};

    db::Database locker(p);
    db::Database contender(p);

    locker.execute("CREATE TABLE t(id INTEGER PRIMARY KEY, v INTEGER);");
    locker.execute("INSERT INTO t(id, v) VALUES(1, 1);");

    contender.setBusyTimeout(50);

    locker.execute("BEGIN EXCLUSIVE TRANSACTION;");

    const auto start = std::chrono::steady_clock::now();

    try
    {
        contender.execute("UPDATE t SET v=2 WHERE id=1;");
        FAIL() << "Expected db::SqliteError due to lock contention";
    }
    catch (const db::SqliteError& ex)
    {
        expect_sqlite_error_contains(
            ex,
            "sqlite execute failed",
            "UPDATE t SET v=2"
        );
    }

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start
    );
    EXPECT_LT(elapsed.count(), 2000);

    EXPECT_NO_THROW(locker.execute("ROLLBACK;"));
}

TEST(Database, OpenAfterCloseReopensAndOperates)
{
    const auto p = unique_temp_db_path();
    TempDbFile cleanup{p};

    db::Database db(p);
    db.close();
    EXPECT_FALSE(db.isOpen());

    EXPECT_NO_THROW(db.open(p.string()));
    EXPECT_TRUE(db.isOpen());

    EXPECT_NO_THROW(db.execute("CREATE TABLE t(x INTEGER);"));
}

TEST(Database, OpenInvalidPathThrows)
{
    const auto dir =
        std::filesystem::temp_directory_path() / "molartracker_db_open_dir";
    std::error_code ec;
    std::filesystem::create_directories(dir, ec);

    try
    {
        db::Database db(dir);
        FAIL() << "Expected db::SqliteError when opening directory as database";
    }
    catch (const db::SqliteError& ex)
    {
        expect_sqlite_error_contains(ex, "Failed to open sqlite database");
    }

    std::filesystem::remove_all(dir, ec);
}
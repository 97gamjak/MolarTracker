#ifndef __TESTS__DB__TEST_STATEMENT_HPP__
#define __TESTS__DB__TEST_STATEMENT_HPP__

#include <gtest/gtest.h>
#include <sqlite3.h>

#include <string>
#include <utility>

#include "db/statement.hpp"

namespace db::test
{
    class SqliteDb
    {
       public:
        SqliteDb()
        {
            const int rc = sqlite3_open(":memory:", &_db);
            EXPECT_EQ(rc, SQLITE_OK);
            EXPECT_NE(_db, nullptr);
        }

        ~SqliteDb()
        {
            if (_db)
                sqlite3_close(_db);
        }

        SqliteDb(SqliteDb const&)            = delete;
        SqliteDb& operator=(SqliteDb const&) = delete;

        sqlite3* native() const noexcept { return _db; }

        void exec(const std::string& sql) const
        {
            char*     err = nullptr;
            const int rc =
                sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &err);

            if (rc != SQLITE_OK)
            {
                std::string msg = err ? err : "sqlite3_exec failed";
                sqlite3_free(err);
                FAIL() << msg;
            }
        }

       private:
        sqlite3* _db{nullptr};
    };

    class Prepared
    {
       public:
        Prepared(sqlite3* db, const std::string& sql)
        {
            const int rc =
                sqlite3_prepare_v2(db, sql.c_str(), -1, &_stmt, nullptr);
            EXPECT_EQ(rc, SQLITE_OK);
            EXPECT_NE(_stmt, nullptr);
        }

        ~Prepared()
        {
            if (_stmt)
                sqlite3_finalize(_stmt);
        }

        Prepared(Prepared const&)            = delete;
        Prepared& operator=(Prepared const&) = delete;

        Prepared(Prepared&& other) noexcept : _stmt(other._stmt)
        {
            other._stmt = nullptr;
        }

        Prepared& operator=(Prepared&& other) noexcept
        {
            if (this != &other)
            {
                if (_stmt)
                    sqlite3_finalize(_stmt);
                _stmt       = other._stmt;
                other._stmt = nullptr;
            }
            return *this;
        }

        sqlite3_stmt* release() noexcept
        {
            sqlite3_stmt* tmp = _stmt;
            _stmt             = nullptr;
            return tmp;
        }

       private:
        sqlite3_stmt* _stmt{nullptr};
    };

    inline db::Statement make_statement(
        sqlite3*    db,
        Prepared&   prepared,
        std::string sql_for_errors
    )
    {
        return db::Statement{db, prepared.release(), std::move(sql_for_errors)};
    }

    class StatementFixture : public ::testing::Test
    {
       protected:
        SqliteDb db;
    };
}   // namespace db::test

#endif   // __TESTS__DB__TEST_STATEMENT_HPP__
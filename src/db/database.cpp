#include "database.hpp"

#include <sqlite3.h>

#include <filesystem>
#include <format>
#include <string>
#include <utility>

#include "db_exception.hpp"
#include "statement.hpp"

#define __LOG_CATEGORY__ LogCategory::database
#include "logging/log_macros.hpp"

namespace db
{
    /**
     * @brief Construct a new Database:: Database object
     *
     * @param dbPath
     */
    Database::Database(const std::filesystem::path& dbPath)
    {
        std::filesystem::path path = dbPath;
        if (!path.is_absolute())
            path = std::filesystem::absolute(path);

        open(path.string());
    }

    /**
     * @brief Destroy the Database:: Database object
     *
     */
    Database::~Database() { close(); }

    /**
     * @brief Move constructor
     *
     * @param other
     */
    Database::Database(Database&& other) { _moveFrom(std::move(other)); }

    /**
     * @brief Move assignment operator
     *
     * @param other
     * @return Database&
     */
    Database& Database::operator=(Database&& other)
    {
        if (this != &other)
        {
            close();
            _moveFrom(std::move(other));
        }

        return *this;
    }

    /**
     * @brief Move helper method
     *
     * @param other
     */
    void Database::_moveFrom(Database&& other)
    {
        _db     = other._db;
        _dbPath = std::move(other._dbPath);

        other._db = nullptr;
        other._dbPath.clear();
    }

    /**
     * @brief open the database at the specified path
     *
     * @param dbPath
     */
    void Database::open(const std::string& dbPath)
    {
        close();

        sqlite3* openedHandle = nullptr;

        const auto result = sqlite3_open_v2(
            dbPath.c_str(),
            &openedHandle,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
            nullptr
        );

        if (result != SQLITE_OK)
        {
            std::string msg  = "Failed to open sqlite database ";
            msg             += '"' + dbPath + '"';

            if (openedHandle != nullptr)
            {
                char const* openMsg = sqlite3_errmsg(openedHandle);
                if (openMsg != nullptr)
                {
                    msg += " : ";
                    msg += openMsg;
                }
                sqlite3_close(openedHandle);
            }

            throw SqliteError(msg);
        }

        _db     = openedHandle;
        _dbPath = dbPath;

        enableForeignKeys(true);
        setBusyTimeout(5000);
    }

    /**
     * @brief close the database
     *
     */
    void Database::close()
    {
        if (_db != nullptr)
        {
            sqlite3_close(_db);
            _db = nullptr;
        }

        _dbPath.clear();
    }

    /**
     * @brief check if the database is open
     *
     * @return true
     * @return false
     */
    bool Database::isOpen() const { return _db != nullptr; }

    /**
     * @brief get the native sqlite3 database handle
     *
     * @return sqlite3*
     */
    sqlite3* Database::nativeHandle() const { return _db; }

    /**
     * @brief execute a SQL statement
     *
     * @param sql
     */
    void Database::execute(std::string_view sql)
    {
        _ensureOpen();

        char* rawError = nullptr;

        const auto result = sqlite3_exec(
            _db,
            std::string(sql).c_str(),
            nullptr,
            nullptr,
            &rawError
        );

        if (result != SQLITE_OK)
        {
            std::string msg = "sqlite execute failed";

            if (rawError != nullptr)
            {
                msg += " : ";
                msg += rawError;
                sqlite3_free(rawError);
            }
            else
            {
                msg += " : ";
                msg += _sqliteErrorMessage();
            }

            msg += " | sql: ";
            msg += std::string(sql);

            throw SqliteError(msg);
        }
    }

    /**
     * @brief prepare a SQL statement for execution
     *
     * @param sql
     * @return Statement
     */
    Statement Database::prepare(std::string_view sql)
    {
        _ensureOpen();

        sqlite3_stmt* preparedStatement = nullptr;

        const auto result = sqlite3_prepare_v2(
            _db,
            sql.data(),
            static_cast<int>(sql.size()),
            &preparedStatement,
            nullptr
        );

        if (result != SQLITE_OK)
        {
            std::string msg  = "sqlite prepare failed : ";
            msg             += _sqliteErrorMessage();
            msg             += " | sql: ";
            msg             += std::string(sql);

            throw SqliteError(msg);
        }

        LOG_DEBUG(std::format("Prepared SQL statement: {}", sql));

        return Statement{_db, preparedStatement, std::string(sql)};
    }

    /**
     * @brief get the row ID of the last inserted row
     *
     * @return std::int64_t
     */
    std::int64_t Database::lastInsertRowid() const
    {
        _ensureOpen();
        return static_cast<std::int64_t>(sqlite3_last_insert_rowid(_db));
    }

    /**
     * @brief get the number of rows changed by the last executed statement
     *
     * @return int
     */
    int Database::changes() const
    {
        _ensureOpen();
        return sqlite3_changes(_db);
    }

    /**
     * @brief set the busy timeout in milliseconds
     *
     * @param timeout_milliseconds
     */
    void Database::setBusyTimeout(int timeout_milliseconds)
    {
        _ensureOpen();

        const auto result = sqlite3_busy_timeout(_db, timeout_milliseconds);

        if (result != SQLITE_OK)
            throw SqliteError(
                "Failed to set sqlite busy timeout : " + _sqliteErrorMessage()
            );
    }

    /**
     * @brief enable or disable foreign key support
     *
     * @param enabled
     */
    void Database::enableForeignKeys(bool enabled)
    {
        if (enabled)
            execute("PRAGMA foreign_keys = ON;");
        else
            execute("PRAGMA foreign_keys = OFF;");
    }

    //
    //
    // PRIVATE HELPER METHODS
    //
    //

    /**
     * @brief ensure that the database is open
     *
     */
    void Database::_ensureOpen() const
    {
        if (_db == nullptr)
            throw SqliteError("Database is not open");
    }

    /**
     * @brief get the last sqlite error message
     *
     * @return std::string
     */
    std::string Database::_sqliteErrorMessage() const
    {
        if (_db == nullptr)
            return "sqlite error: database handle is null";

        auto const* msg = sqlite3_errmsg(_db);

        if (msg == nullptr)
            return "sqlite error: sqlite3_errmsg returned null";

        return std::string{msg};
    }

}   // namespace db

#include "db/database.hpp"

#include <sqlite3.h>

#include <cstdint>
#include <filesystem>
#include <format>
#include <ranges>
#include <string>
#include <utility>

#include "config/constants.hpp"
#include "db/db_exception.hpp"
#include "db/statement.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("DB.Database");

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
    Database::Database(Database&& other) noexcept
    {
        _moveFrom(std::move(other));
    }

    /**
     * @brief Move assignment operator
     *
     * @param other
     * @return Database&
     */
    Database& Database::operator=(Database&& other) noexcept
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
    // NOLINTNEXTLINE(cppcoreguidelines-rvalue-reference-param-not-moved)
    void Database::_moveFrom(Database&& other)
    {
        _db         = std::exchange(other._db, nullptr);
        _dbPath     = std::move(other._dbPath);
        _executions = std::move(other._executions);

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

        _db     = _open(dbPath);
        _dbPath = dbPath;

        enableForeignKeys(true);
        setBusyTimeout(Constants::getDbBusyTimeoutMs());
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

        constexpr size_t MAX_EXECUTIONS_HISTORY = 1000;
        _executions.emplace_back(sql);
        if (_executions.size() > MAX_EXECUTIONS_HISTORY)
        {
            _executions.erase(
                _executions.begin(),
                _executions.begin() +
                    (static_cast<
                        std::ranges::range_difference_t<decltype(_executions)>>(
                        _executions.size() - MAX_EXECUTIONS_HISTORY
                    ))
            );
        }
    }

    /**
     * @brief Begins a database transaction.
     *
     * @param immediate If true, starts an immediate transaction.
     */
    void Database::begin(bool immediate)
    {
        if (immediate)
            execute("BEGIN IMMEDIATE;");
        else
            execute("BEGIN;");

        _transactionStarted = true;
    }

    /**
     * @brief Checks if a database transaction is currently active.
     *
     * @return true if a transaction is active, false otherwise.
     */
    bool Database::isTransactionStarted() const { return _transactionStarted; }

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
    std::optional<std::int64_t> Database::getLastInsertRowid() const
    {
        _ensureOpen();
        const auto lastRowId = sqlite3_last_insert_rowid(_db);

        if (lastRowId == 0)
            return std::nullopt;

        return static_cast<int64_t>(lastRowId);
    }

    /**
     * @brief get the number of rows changed by the last executed statement
     *
     * @return int64_t
     */
    std::int64_t Database::getNumberOfLastChanges() const
    {
        _ensureOpen();

        return static_cast<int64_t>(sqlite3_changes64(_db));
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

    /**
     * @brief Query a single integer value from the database
     *
     * @param sql The SQL query to execute
     * @return int The queried integer value
     */
    int Database::queryInt(std::string_view sql)
    {
        auto       statement = prepare(sql);
        const auto result    = statement.step();

        if (result != StepResult::RowAvailable)
        {
            throw SqliteError(
                "queryInt: expected a row but got none | sql: " +
                std::string(sql)
            );
        }

        return static_cast<int>(statement.columnInt64(0));
    }

    /**
     * @brief Create a backup copy of the database
     *
     */
    void Database::makeBackup()
    {
        _ensureOpen();

        sqlite3* backupDb = _open(_dbPath + ".bck");

        // Use SQLite's backup API to create a backup copy of the database
        sqlite3_backup* backup =
            sqlite3_backup_init(backupDb, "main", _db, "main");

        if (backup == nullptr)
        {
            throw SqliteError(
                "Failed to initialize backup: " + _sqliteErrorMessage()
            );
        }

        // Perform the backup
        sqlite3_backup_step(backup, -1);
        sqlite3_backup_finish(backup);

        sqlite3_close(backupDb);
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

    /**
     * @brief Open a SQLite database connection
     *
     * @param path The path to the database file
     * @return sqlite3* The opened database handle
     */
    sqlite3* Database::_open(const std::string& path)
    {
        sqlite3* openedHandle = nullptr;

        const auto result = sqlite3_open_v2(
            path.c_str(),
            &openedHandle,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
            nullptr
        );

        if (result != SQLITE_OK)
        {
            std::string msg  = "Failed to open sqlite database ";
            msg             += '"' + path + '"';

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

        return openedHandle;
    }

}   // namespace db

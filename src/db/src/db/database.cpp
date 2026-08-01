#include "db/database.hpp"

#include <sqlite3.h>

#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <ranges>
#include <string>
#include <utility>

#include "config/constants/constants.hpp"
#include "db/db_exception.hpp"
#include "db/statement.hpp"
#include "error/crud_error.hpp"
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

        if (!std::filesystem::exists(path))
        {
            LOG_INFO("Database file does not exist at path: " + path.string());
            LOG_INFO("Creating new database file at path: " + path.string());
            std::ofstream ofs(path);
            if (!ofs)
            {
                throw SqliteError(
                    std::format(
                        "Failed to create database file at path: {}",
                        path.string()
                    )
                );
            }
        }

        const auto error = open(path.string());

        if (!error)
        {
            throw SqliteError(
                std::format(
                    "Failed to open database at path: {} | Error: {}",
                    path.string(),
                    error.error().toString()
                )
            );
        }
    }

    /**
     * @brief Destroy the Database:: Database object
     *
     */
    Database::~Database()
    {
        try
        {
            close();
        }
        catch (const SqliteError& e)
        {
            LOG_ERROR(
                std::string{"Failed to close database during destruction: "} +
                e.what()
            );
        }
    }

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
            try
            {
                close();
            }
            catch (const SqliteError& e)
            {
                LOG_ERROR(
                    std::string{"Failed to close database during move: "} +
                    e.what()
                );
            }
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
        _db                 = std::exchange(other._db, nullptr);
        _dbPath             = std::move(other._dbPath);
        _executions         = std::move(other._executions);
        _transactionStarted = other._transactionStarted;

        other._dbPath.clear();
    }

    /**
     * @brief open the database at the specified path
     *
     * @param dbPath
     *
     * @return DatabaseResult<void>
     */
    DatabaseResult<void> Database::open(const std::string& dbPath)
    {
        close();

        if (!std::filesystem::exists(dbPath))
        {
            return DatabaseError{
                DatabaseErrorType::OpeningFailed,
                std::format("Database file does not exist at path: {}", dbPath)
            };
        }

        _db     = _open(dbPath);
        _dbPath = dbPath;

        LOG_DEBUG("Opened database at path: " + dbPath);

        const auto fkResult = enableForeignKeys(true);
        if (!fkResult)
        {
            return fkResult.error().convert(
                DatabaseErrorType::OpeningFailed,
                "Failed to enable foreign keys"
            );
        }

        setBusyTimeout(Constants::getDbBusyTimeoutMs());
        return {};
    }

    /**
     * @brief close the database
     *
     */
    void Database::close()
    {
        if (_db != nullptr)
        {
            int returnCode = sqlite3_close(_db);
            if (returnCode != SQLITE_OK)
            {
                throw SqliteError(
                    std::format(
                        "sqlite3_close failed: {} ({})",
                        returnCode,
                        sqlite3_errmsg(_db)
                    )
                );
            }

            LOG_DEBUG("Closed database at path: " + _dbPath);
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
     *
     * @return DatabaseResult<void>
     */
    DatabaseResult<void> Database::execute(std::string_view sql)
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

            return DatabaseError{DatabaseErrorType::SqliteError, msg};
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

        LOG_DEBUG(std::format("Executed SQL statement: {}", sql));

        return {};
    }

    /**
     * @brief Begins a database transaction.
     *
     * @param immediate If true, starts an immediate transaction.
     *
     * @return DatabaseResult<void> Returns a DatabaseResult indicating success
     * or failure.
     */
    DatabaseResult<void> Database::begin(bool immediate)
    {
        DatabaseResult<void> result;
        const char*          statement = "BEGIN;";

        if (immediate)
            statement = "BEGIN IMMEDIATE;";

        result = execute(statement);

        if (!result)
        {
            return result.error().convert(
                DatabaseErrorType::BeginTransactionFailed,
                "Failed to begin transaction"
            );
        }

        _transactionStarted = true;

        return {};
    }

    /**
     * @brief Commits the current transaction.
     *
     * @return DatabaseResult<void> Returns a DatabaseResult indicating success
     * or failure.
     */
    DatabaseResult<void> Database::commit()
    {
        const auto result = execute("COMMIT;");
        if (!result)
        {
            return result.error().convert(
                DatabaseErrorType::CommitTransactionFailed,
                "Failed to commit transaction"
            );
        }

        _transactionStarted = false;
        return {};
    }

    /**
     * @brief Rolls back the current transaction.
     *
     * @return DatabaseResult<void> Returns a DatabaseResult indicating success
     */
    DatabaseResult<void> Database::rollback()
    {
        const auto result = execute("ROLLBACK;");
        if (!result)
        {
            return result.error().convert(
                DatabaseErrorType::RollbackTransactionFailed,
                "Failed to rollback transaction"
            );
        }

        _transactionStarted = false;
        return {};
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
     *
     * @return DatabaseResult<void>
     */
    DatabaseResult<void> Database::enableForeignKeys(bool enabled)
    {
        const auto statement =
            std::format("PRAGMA foreign_keys = {};", enabled ? "ON" : "OFF");

        const auto result = execute(statement);

        if (!result)
        {
            return result.error().convert(
                DatabaseErrorType::ForeignKeyPragmaChangeFailed,
                "Failed to enable foreign keys"
            );
        }

        return {};
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

    /**
     * @brief Get the path to the database file
     *
     * @return std::string
     */
    [[nodiscard]] std::string Database::getDBPath() const { return _dbPath; }

}   // namespace db

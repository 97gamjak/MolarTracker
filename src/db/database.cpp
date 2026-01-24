#include "database.hpp"

#include <sqlite3.h>

#include <filesystem>
#include <string>
#include <utility>

#include "db_exception.hpp"
#include "statement.hpp"

namespace db
{
    /**
     * @brief Construct a new Database:: Database object
     *
     * @param db_path
     */
    Database::Database(const std::filesystem::path& db_path)
    {
        std::filesystem::path path = db_path;
        if (!path.is_absolute())
        {
            path = std::filesystem::absolute(path);
        }
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
    Database::Database(Database&& other) { _move_from(std::move(other)); }

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
            _move_from(std::move(other));
        }

        return *this;
    }

    /**
     * @brief Move helper method
     *
     * @param other
     */
    void Database::_move_from(Database&& other)
    {
        _db      = other._db;
        _db_path = std::move(other._db_path);

        other._db = nullptr;
        other._db_path.clear();
    }

    /**
     * @brief open the database at the specified path
     *
     * @param db_path
     */
    void Database::open(const std::string& db_path)
    {
        close();

        sqlite3* opened_handle = nullptr;

        const auto result = sqlite3_open_v2(
            db_path.c_str(),
            &opened_handle,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
            nullptr
        );

        if (result != SQLITE_OK)
        {
            std::string msg  = "Failed to open sqlite database ";
            msg             += '"' + db_path + '"';

            if (opened_handle != nullptr)
            {
                char const* open_msg = sqlite3_errmsg(opened_handle);
                if (open_msg != nullptr)
                {
                    msg += " : ";
                    msg += open_msg;
                }
                sqlite3_close(opened_handle);
            }

            throw SqliteError(msg);
        }

        _db      = opened_handle;
        _db_path = db_path;

        enable_foreign_keys(true);
        set_busy_timeout_ms(5000);
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

        _db_path.clear();
    }

    /**
     * @brief check if the database is open
     *
     * @return true
     * @return false
     */
    bool Database::is_open() const { return _db != nullptr; }

    /**
     * @brief get the native sqlite3 database handle
     *
     * @return sqlite3*
     */
    sqlite3* Database::native_handle() const { return _db; }

    /**
     * @brief execute a SQL statement
     *
     * @param sql
     */
    void Database::execute(std::string_view sql)
    {
        _ensure_open();

        char* raw_error_message = nullptr;

        const auto result = sqlite3_exec(
            _db,
            std::string(sql).c_str(),
            nullptr,
            nullptr,
            &raw_error_message
        );

        if (result != SQLITE_OK)
        {
            std::string msg = "sqlite execute failed";

            if (raw_error_message != nullptr)
            {
                msg += " : ";
                msg += raw_error_message;
                sqlite3_free(raw_error_message);
            }
            else
            {
                msg += " : ";
                msg += _sqlite_error_message();
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
        _ensure_open();

        sqlite3_stmt* prepared_statement = nullptr;

        const auto result = sqlite3_prepare_v2(
            _db,
            sql.data(),
            static_cast<int>(sql.size()),
            &prepared_statement,
            nullptr
        );

        if (result != SQLITE_OK)
        {
            std::string msg  = "sqlite prepare failed : ";
            msg             += _sqlite_error_message();
            msg             += " | sql: ";
            msg             += std::string(sql);

            throw SqliteError(msg);
        }

        return Statement{_db, prepared_statement, std::string(sql)};
    }

    /**
     * @brief get the row ID of the last inserted row
     *
     * @return std::int64_t
     */
    std::int64_t Database::last_insert_rowid() const
    {
        _ensure_open();
        return static_cast<std::int64_t>(sqlite3_last_insert_rowid(_db));
    }

    /**
     * @brief get the number of rows changed by the last executed statement
     *
     * @return int
     */
    int Database::changes() const
    {
        _ensure_open();
        return sqlite3_changes(_db);
    }

    /**
     * @brief set the busy timeout in milliseconds
     *
     * @param timeout_milliseconds
     */
    void Database::set_busy_timeout_ms(int timeout_milliseconds)
    {
        _ensure_open();

        const auto result = sqlite3_busy_timeout(_db, timeout_milliseconds);

        if (result != SQLITE_OK)
            throw SqliteError(
                "Failed to set sqlite busy timeout : " + _sqlite_error_message()
            );
    }

    /**
     * @brief enable or disable foreign key support
     *
     * @param enabled
     */
    void Database::enable_foreign_keys(bool enabled)
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
    void Database::_ensure_open() const
    {
        if (_db == nullptr)
            throw SqliteError("Database is not open");
    }

    /**
     * @brief get the last sqlite error message
     *
     * @return std::string
     */
    std::string Database::_sqlite_error_message() const
    {
        if (_db == nullptr)
            return "sqlite error: database handle is null";

        auto const* msg = sqlite3_errmsg(_db);

        if (msg == nullptr)
            return "sqlite error: sqlite3_errmsg returned null";

        return std::string{msg};
    }

}   // namespace db

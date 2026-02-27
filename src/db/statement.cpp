#include "statement.hpp"

#include <sqlite3.h>

#include <string>
#include <utility>

#include "db_exception.hpp"

namespace db
{

    /**
     * @brief check if the step result is valid
     *
     * @param result
     * @return true
     * @return false
     */
    bool StepResultMeta::isValid(int result)
    {
        const auto value = static_cast<StepResult>(result);
        switch (value)
        {
            case StepResult::RowAvailable:
            case StepResult::Done:
                return true;
            default:
                return false;
        }
    }

    /**
     * @brief Construct a new Statement:: Statement object
     *
     * @param db
     * @param statement
     * @param sqlForErrors
     */
    Statement::Statement(
        sqlite3*      db,
        sqlite3_stmt* statement,
        std::string   sqlForErrors
    )
        : _db(db), _statement(statement), _sqlForErrors(std::move(sqlForErrors))
    {
    }

    /**
     * @brief Destroy the Statement:: Statement object
     *
     */
    Statement::~Statement() { _finalize(); }

    /**
     * @brief Move constructor
     *
     * @param other
     */
    Statement::Statement(Statement&& other) noexcept
    {
        _moveFrom(std::move(other));
    }

    /**
     * @brief Move assignment operator
     *
     * @param other
     * @return Statement&
     */
    Statement& Statement::operator=(Statement&& other) noexcept
    {
        if (this != &other)
        {
            _finalize();
            _moveFrom(std::move(other));
        }

        return *this;
    }

    /**
     * @brief check if the statement is valid (i.e., statement handle is not
     * null)
     *
     * @return true
     * @return false
     */
    bool Statement::isValid() const { return _statement != nullptr; }

    /**
     * @brief take a step in the execution of the prepared statement
     *
     * @return StepResult
     */
    StepResult Statement::step()
    {
        _ensureValid();

        const auto result = sqlite3_step(_statement);

        if (StepResultMeta::isValid(result))
            return static_cast<StepResult>(result);

        throw _generateError("step", result);
    }

    /**
     * @brief execute the statement to completion, expecting no rows to be
     * produced
     *
     */
    void Statement::executeToCompletion()
    {
        const auto result = step();

        if (result != StepResult::Done)
            throw SqliteError(
                "Statement was expected to finish but produced a row"
            );
    }

    /**
     * @brief reset the statement to its initial state, clearing any bound
     * parameters
     *
     */
    void Statement::reset()
    {
        _ensureValid();

        const auto reset_result = sqlite3_reset(_statement);
        if (reset_result != SQLITE_OK)
            throw _generateError("reset", reset_result);

        const auto clear_result = sqlite3_clear_bindings(_statement);
        if (clear_result != SQLITE_OK)
            throw _generateError("clear_bindings", clear_result);
    }

    /**
     * @brief bind a null value to the specified parameter index (1-based)
     *
     * @param index
     */
    void Statement::bindNull(const int index)
    {
        _ensureValid();

        const auto result = sqlite3_bind_null(_statement, index);

        if (result != SQLITE_OK)
            throw _generateError("bindNull", result);
    }

    /**
     * @brief bind an int64 value to the specified parameter index (1-based)
     *
     * @param index
     * @param value
     */
    void Statement::bindInt64(const int index, const std::int64_t value)
    {
        _ensureValid();
        const auto sql_value = static_cast<sqlite3_int64>(value);

        const auto result = sqlite3_bind_int64(_statement, index, sql_value);

        if (result != SQLITE_OK)
            throw _generateError("bindInt64", result);
    }

    /**
     * @brief bind a double value to the specified parameter index (1-based)
     *
     * @param index
     * @param value
     */
    void Statement::bindDouble(const int index, const double value)
    {
        _ensureValid();

        const auto result = sqlite3_bind_double(_statement, index, value);
        if (result != SQLITE_OK)
            throw _generateError("bindDouble", result);
    }

    /**
     * @brief bind a text value to the specified parameter index (1-based)
     *
     * @param index
     * @param value
     */
    void Statement::bindText(const int index, const std::string_view value)
    {
        _ensureValid();

        const auto result = sqlite3_bind_text(
            _statement,
            index,
            value.data(),
            static_cast<int>(value.size()),
            SQLITE_TRANSIENT
        );

        if (result != SQLITE_OK)
            throw _generateError("bindText", result);
    }

    /**
     * @brief check if the value in the specified column is null
     *
     * @param col
     * @return true
     * @return false
     */
    bool Statement::columnIsNull(const int col) const
    {
        _ensureValid();
        return sqlite3_column_type(_statement, col) == SQLITE_NULL;
    }

    /**
     * @brief get the int64 value from the specified column
     *
     * @param col
     * @return std::int64_t
     */
    std::int64_t Statement::columnInt64(const int col) const
    {
        _ensureValid();
        return static_cast<std::int64_t>(sqlite3_column_int64(_statement, col));
    }

    /**
     * @brief get the double value from the specified column
     *
     * @param col
     * @return double
     */
    double Statement::columnDouble(const int col) const
    {
        _ensureValid();
        return sqlite3_column_double(_statement, col);
    }

    /**
     * @brief get the text value from the specified column
     *
     * @param col
     * @return std::string
     */
    std::string Statement::columnText(const int col) const
    {
        _ensureValid();

        auto const* bytes     = sqlite3_column_text(_statement, col);
        const auto  byteCount = sqlite3_column_bytes(_statement, col);

        if (bytes == nullptr || byteCount <= 0)
            return {};

        return std::string{
            reinterpret_cast<char const*>(bytes),
            static_cast<std::size_t>(byteCount)
        };
    }

    /**
     * @brief get the native sqlite3_stmt handle (for advanced use cases)
     *
     * @return sqlite3_stmt*
     */
    sqlite3_stmt* Statement::nativeHandle() const { return _statement; }

    //
    //
    // PRIVATE HELPER METHODS
    //
    //

    /**
     * @brief ensure that the statement is valid (i.e., statement handle is not
     * null) and throw an exception if it is not
     *
     */
    void Statement::_ensureValid() const
    {
        if (_statement == nullptr)
            throw SqliteError("Statement is not valid");
    }

    /**
     * @brief generate a SqliteError with a detailed error message based on the
     * given operation and result code
     *
     * @param operation The name of the SQLite operation that failed (e.g.,
     * "step", "reset", etc.)
     * @param result The SQLite result code returned by the failed operation
     * @return SqliteError with a detailed error message
     */
    SqliteError Statement::_generateError(
        const std::string_view operation,
        const int              result
    ) const
    {
        std::string msg;
        msg += "sqlite " + std::string(operation);
        msg += " failed (result=";
        msg += std::to_string(result) + ")";

        if (_db != nullptr)
        {
            char const* sqlMsg = sqlite3_errmsg(_db);
            if (sqlMsg != nullptr)
                msg += " : " + std::string(sqlMsg);
        }

        if (!_sqlForErrors.empty())
            msg += " | sql: " + _sqlForErrors;

        return SqliteError(msg);
    }

    /**
     * @brief move the resources from another Statement object to this one
     *
     * @param other The other Statement object to move from
     */
    void Statement::_moveFrom(Statement&& other)
    {
        _db           = other._db;
        _statement    = other._statement;
        _sqlForErrors = std::move(other._sqlForErrors);

        other._db        = nullptr;
        other._statement = nullptr;
        other._sqlForErrors.clear();
    }

    /**
     * @brief finalize the statement by resetting and finalizing the native
     * sqlite3_stmt handle, and clearing the SQL string for errors
     *
     */
    void Statement::_finalize()
    {
        if (_statement != nullptr)
        {
            sqlite3_reset(_statement);
            sqlite3_finalize(_statement);
            _statement = nullptr;
        }

        _db = nullptr;
        _sqlForErrors.clear();
    }

}   // namespace db

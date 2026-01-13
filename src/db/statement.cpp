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
    bool StepResultMeta::is_valid(int result)
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
     * @param sql_for_errors
     */
    Statement::Statement(
        sqlite3*      db,
        sqlite3_stmt* statement,
        std::string   sql_for_errors
    )
        : _db(db),
          _statement(statement),
          _sql_for_errors(std::move(sql_for_errors))
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
    Statement::Statement(Statement&& other) { _move_from(std::move(other)); }

    /**
     * @brief Move assignment operator
     *
     * @param other
     * @return Statement&
     */
    Statement& Statement::operator=(Statement&& other)
    {
        if (this != &other)
        {
            _finalize();
            _move_from(std::move(other));
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
    bool Statement::is_valid() const { return _statement != nullptr; }

    /**
     * @brief take a step in the execution of the prepared statement
     *
     * @return StepResult
     */
    StepResult Statement::step()
    {
        _ensure_valid();

        const auto result = sqlite3_step(_statement);

        if (StepResultMeta::is_valid(result))
            return static_cast<StepResult>(result);

        throw _generate_error("step", result);
    }

    /**
     * @brief execute the statement to completion, expecting no rows to be
     * produced
     *
     */
    void Statement::execute_to_completion()
    {
        const auto result = step();

        if (result != StepResult::Done)
        {
            throw SqliteError(
                "Statement was expected to finish but produced a row"
            );
        }
    }

    /**
     * @brief reset the statement to its initial state, clearing any bound
     * parameters
     *
     */
    void Statement::reset()
    {
        _ensure_valid();

        const auto reset_result = sqlite3_reset(_statement);
        if (reset_result != SQLITE_OK)
            throw _generate_error("reset", reset_result);

        const auto clear_result = sqlite3_clear_bindings(_statement);
        if (clear_result != SQLITE_OK)
            throw _generate_error("clear_bindings", clear_result);
    }

    /**
     * @brief bind a null value to the specified parameter index (1-based)
     *
     * @param index
     */
    void Statement::bind_null(const int index)
    {
        _ensure_valid();

        const auto result = sqlite3_bind_null(_statement, index);

        if (result != SQLITE_OK)
            throw _generate_error("bind_null", result);
    }

    /**
     * @brief bind an int64 value to the specified parameter index (1-based)
     *
     * @param index
     * @param value
     */
    void Statement::bind_int64(const int index, const std::int64_t value)
    {
        _ensure_valid();
        const auto sql_value = static_cast<sqlite3_int64>(value);

        const auto result = sqlite3_bind_int64(_statement, index, sql_value);

        if (result != SQLITE_OK)
            throw _generate_error("bind_int64", result);
    }

    /**
     * @brief bind a double value to the specified parameter index (1-based)
     *
     * @param index
     * @param value
     */
    void Statement::bind_double(const int index, const double value)
    {
        _ensure_valid();

        const auto result = sqlite3_bind_double(_statement, index, value);
        if (result != SQLITE_OK)
            throw _generate_error("bind_double", result);
    }

    /**
     * @brief bind a text value to the specified parameter index (1-based)
     *
     * @param index
     * @param value
     */
    void Statement::bind_text(const int index, const std::string_view value)
    {
        _ensure_valid();

        const auto result = sqlite3_bind_text(
            _statement,
            index,
            value.data(),
            static_cast<int>(value.size()),
            SQLITE_TRANSIENT
        );

        if (result != SQLITE_OK)
            throw _generate_error("bind_text", result);
    }

    /**
     * @brief check if the value in the specified column is null
     *
     * @param col
     * @return true
     * @return false
     */
    bool Statement::column_is_null(const int col) const
    {
        _ensure_valid();
        return sqlite3_column_type(_statement, col) == SQLITE_NULL;
    }

    /**
     * @brief get the int64 value from the specified column
     *
     * @param col
     * @return std::int64_t
     */
    std::int64_t Statement::column_int64(const int col) const
    {
        _ensure_valid();
        return static_cast<std::int64_t>(sqlite3_column_int64(_statement, col));
    }

    /**
     * @brief get the double value from the specified column
     *
     * @param col
     * @return double
     */
    double Statement::column_double(const int col) const
    {
        _ensure_valid();
        return sqlite3_column_double(_statement, col);
    }

    /**
     * @brief get the text value from the specified column
     *
     * @param col
     * @return std::string
     */
    std::string Statement::column_text(const int col) const
    {
        _ensure_valid();

        auto const* bytes      = sqlite3_column_text(_statement, col);
        const auto  byte_count = sqlite3_column_bytes(_statement, col);

        if (bytes == nullptr || byte_count <= 0)
            return {};

        return std::string{
            reinterpret_cast<char const*>(bytes),
            static_cast<std::size_t>(byte_count)
        };
    }

    sqlite3_stmt* Statement::native_handle() const { return _statement; }

    //
    //
    // PRIVATE HELPER METHODS
    //
    //

    void Statement::_ensure_valid() const
    {
        if (_statement == nullptr)
            throw SqliteError("Statement is not valid");
    }

    SqliteError Statement::_generate_error(
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
            char const* sql_msg = sqlite3_errmsg(_db);
            if (sql_msg != nullptr)
                msg += " : " + std::string(sql_msg);
        }

        if (!_sql_for_errors.empty())
            msg += " | sql: " + _sql_for_errors;

        return SqliteError(msg);
    }

    void Statement::_move_from(Statement&& other)
    {
        _db             = other._db;
        _statement      = other._statement;
        _sql_for_errors = std::move(other._sql_for_errors);

        other._db        = nullptr;
        other._statement = nullptr;
        other._sql_for_errors.clear();
    }

    void Statement::_finalize()
    {
        if (_statement != nullptr)
        {
            sqlite3_reset(_statement);
            sqlite3_finalize(_statement);
            _statement = nullptr;
        }

        _db = nullptr;
        _sql_for_errors.clear();
    }

}   // namespace db

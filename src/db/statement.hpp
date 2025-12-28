#ifndef __DB__STATEMENT_HPP__
#define __DB__STATEMENT_HPP__

#include <sqlite3.h>

#include <cstdint>
#include <string>
#include <string_view>

namespace db
{
    class SqliteError;

    /**
     * @brief Result of stepping a prepared statement
     *
     */
    enum class StepResult
    {
        RowAvailable = SQLITE_ROW,
        Done         = SQLITE_DONE
    };

    /**
     * @brief Metadata helper for StepResult
     *
     */
    class StepResultMeta
    {
       public:
        static bool is_valid(int result);
    };

    /**
     * @brief A wrapper around an SQLite prepared statement
     *
     */
    class Statement
    {
       private:
        sqlite3*      _db{nullptr};
        sqlite3_stmt* _statement{nullptr};
        std::string   _sql_for_errors{};

       public:
        Statement() = default;
        Statement(
            sqlite3*      database_handle,
            sqlite3_stmt* statement_handle,
            std::string   sql_for_errors
        );

        ~Statement();

        Statement(Statement const&)            = delete;
        Statement& operator=(Statement const&) = delete;

        Statement(Statement&& other) noexcept;
        Statement& operator=(Statement&& other) noexcept;

        [[nodiscard]] bool is_valid() const noexcept;

        [[nodiscard]] StepResult step();
        void                     execute_to_completion();
        void                     reset();

        void bind_null(const int index);
        void bind_int64(const int index, std::int64_t value);
        void bind_double(const int index, double value);
        void bind_text(const int index, std::string_view value);

        [[nodiscard]] bool         column_is_null(const int col) const;
        [[nodiscard]] std::int64_t column_int64(const int col) const;
        [[nodiscard]] double       column_double(const int col) const;
        [[nodiscard]] std::string  column_text(const int col) const;

        [[nodiscard]] sqlite3_stmt* native_handle() const noexcept;

       private:   // PRIVATE HELPER METHODS
        void        _ensure_valid() const;
        SqliteError _generate_error(
            std::string_view operation,
            const int        result
        ) const;

        void _finalize() noexcept;
        void _move_from(Statement&& other) noexcept;
    };
}   // namespace db

#endif   // __DB__STATEMENT_HPP__

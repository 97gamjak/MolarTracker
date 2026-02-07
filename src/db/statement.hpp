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
        std::string   _sqlForErrors{};

       public:
        Statement() = default;
        Statement(
            sqlite3*      database_handle,
            sqlite3_stmt* statement_handle,
            std::string   sqlForErrors
        );

        ~Statement();

        Statement(Statement const&)            = delete;
        Statement& operator=(Statement const&) = delete;

        Statement(Statement&& other);
        Statement& operator=(Statement&& other);

        [[nodiscard]] bool is_valid() const;

        [[nodiscard]] StepResult step();
        void                     executeToCompletion();
        void                     reset();

        void bindNull(const int index);
        void bindInt64(const int index, std::int64_t value);
        void bindDouble(const int index, double value);
        void bindText(const int index, std::string_view value);

        [[nodiscard]] bool         columnIsNull(const int col) const;
        [[nodiscard]] std::int64_t columnInt64(const int col) const;
        [[nodiscard]] double       columnDouble(const int col) const;
        [[nodiscard]] std::string  columnText(const int col) const;

        [[nodiscard]] sqlite3_stmt* nativeHandle() const;

       private:   // PRIVATE HELPER METHODS
        void        _ensureValid() const;
        SqliteError _generateError(
            std::string_view operation,
            const int        result
        ) const;

        void _finalize();
        void _moveFrom(Statement&& other);
    };
}   // namespace db

#endif   // __DB__STATEMENT_HPP__

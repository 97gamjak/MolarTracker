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
    enum class StepResult : std::uint8_t
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
        /// Pointer to the SQLite database connection (not owned by this class)
        sqlite3* _db{nullptr};

        /// Pointer to the prepared statement handle (owned by this class)
        sqlite3_stmt* _statement{nullptr};

        /// The SQL string used to prepare this statement, stored for error
        /// reporting purposes
        std::string _sqlForErrors;

       public:
        Statement() = default;
        Statement(
            sqlite3*      db,
            sqlite3_stmt* statement,
            std::string   sqlForErrors
        );

        ~Statement();

        Statement(Statement const&)            = delete;
        Statement& operator=(Statement const&) = delete;

        Statement(Statement&& other) noexcept;
        Statement& operator=(Statement&& other) noexcept;

        [[nodiscard]] bool is_valid() const;

        [[nodiscard]] StepResult step();
        void                     executeToCompletion();
        void                     reset();

        void bindNull(int index);
        void bindInt64(int index, std::int64_t value);
        void bindDouble(int index, double value);
        void bindText(int index, std::string_view value);

        [[nodiscard]] bool         columnIsNull(int col) const;
        [[nodiscard]] std::int64_t columnInt64(int col) const;
        [[nodiscard]] double       columnDouble(int col) const;
        [[nodiscard]] std::string  columnText(int col) const;

        [[nodiscard]] sqlite3_stmt* nativeHandle() const;

       private:   // PRIVATE HELPER METHODS
        void                      _ensureValid() const;
        [[nodiscard]] SqliteError _generateError(
            std::string_view operation,
            int              result
        ) const;

        void _finalize();
        void _moveFrom(Statement&& other);
    };
}   // namespace db

#endif   // __DB__STATEMENT_HPP__

#ifndef __DB__DATABASE_HPP__
#define __DB__DATABASE_HPP__

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

struct sqlite3;   // Forward declaration

namespace db
{
    class Statement;   // Forward declaration

    /**
     * @brief A wrapper around an SQLite database connection
     *
     */
    class Database
    {
       private:
        sqlite3*    _db{nullptr};
        std::string _db_path{};

       public:
        Database() = delete;
        explicit Database(const std::filesystem::path& db_path);

        ~Database();

        Database(Database const&)            = delete;
        Database& operator=(Database const&) = delete;

        Database(Database&& other);
        Database& operator=(Database&& other);

        void open(const std::string& db_path);
        void close();

        [[nodiscard]] bool     is_open() const;
        [[nodiscard]] sqlite3* native_handle() const;

        void execute(std::string_view sql);

        [[nodiscard]] Statement prepare(std::string_view sql);

        [[nodiscard]] std::int64_t last_insert_rowid() const;
        [[nodiscard]] int          changes() const;

        void set_busy_timeout_ms(int timeout_milliseconds);
        void enable_foreign_keys(bool enabled);

       private:   // PRIVATE HELPER METHODS
        void                      _ensure_open() const;
        [[nodiscard]] std::string _sqlite_error_message() const;
        void                      _move_from(Database&& other);
    };
}   // namespace db

#endif   // __DB__DATABASE_HPP__
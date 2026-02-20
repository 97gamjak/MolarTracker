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
        /// The native SQLite database handle
        sqlite3* _db{nullptr};

        /// The path to the database file
        std::string _dbPath{};

       public:
        Database() = delete;
        explicit Database(const std::filesystem::path& db_path);

        ~Database();

        Database(Database const&)            = delete;
        Database& operator=(Database const&) = delete;

        Database(Database&& other) noexcept;
        Database& operator=(Database&& other) noexcept;

        void open(const std::string& db_path);
        void close();

        [[nodiscard]] bool     isOpen() const;
        [[nodiscard]] sqlite3* nativeHandle() const;

        void execute(std::string_view sql);

        [[nodiscard]] Statement prepare(std::string_view sql);

        [[nodiscard]] std::int64_t lastInsertRowid() const;
        [[nodiscard]] int          changes() const;

        void setBusyTimeout(int timeout_milliseconds);
        void enableForeignKeys(bool enabled);

       private:   // PRIVATE HELPER METHODS
        void                      _ensureOpen() const;
        [[nodiscard]] std::string _sqliteErrorMessage() const;
        void                      _moveFrom(Database&& other);
    };
}   // namespace db

#endif   // __DB__DATABASE_HPP__
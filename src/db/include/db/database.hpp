#ifndef __DB__INCLUDE__DB__DATABASE_HPP__
#define __DB__INCLUDE__DB__DATABASE_HPP__

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

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
        std::string _dbPath;

        /// Log of executed SQL statements for debugging or auditing purposes
        std::vector<std::string> _executions;

        /// Indicates whether a database transaction is currently active
        bool _transactionStarted = false;

       public:
        Database() = delete;
        explicit Database(const std::filesystem::path& dbPath);

        ~Database();

        Database(Database const&)            = delete;
        Database& operator=(Database const&) = delete;

        Database(Database&& other) noexcept;
        Database& operator=(Database&& other) noexcept;

        void open(const std::string& dbPath);
        void close();

        [[nodiscard]] bool     isOpen() const;
        [[nodiscard]] sqlite3* nativeHandle() const;

        void execute(std::string_view sql);

        [[nodiscard]] Statement prepare(std::string_view sql);

        [[nodiscard]] std::optional<std::int64_t> getLastInsertRowid() const;
        [[nodiscard]] std::int64_t getNumberOfLastChanges() const;

        void setBusyTimeout(int timeout_milliseconds);
        void enableForeignKeys(bool enabled);

        [[nodiscard]] int queryInt(std::string_view sql);

        void makeBackup();

        void begin(bool immediate);

        [[nodiscard]] bool isTransactionStarted() const;
        void               commit();
        void               rollback();

        [[nodiscard]] std::string getDBPath() const;

       private:   // PRIVATE HELPER METHODS
        void                      _ensureOpen() const;
        [[nodiscard]] std::string _sqliteErrorMessage() const;
        void                      _moveFrom(Database&& other);

        [[nodiscard]] static sqlite3* _open(const std::string& path);
    };
}   // namespace db

#endif   // __DB__INCLUDE__DB__DATABASE_HPP__
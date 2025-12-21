#include "database.hpp"

#include <sqlite3.h>

#include <stdexcept>

Database::Database(const std::string path)
{
    if (sqlite3_open(path.c_str(), &_db) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to open database");
    }
}

Database::~Database()
{
    if (_db)
    {
        sqlite3_close(_db);
    }
}

void Database::exec(const std::string& sql)
{
    char* err = nullptr;
    if (sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK)
    {
        std::string msg = err ? err : "unknown sqlite error";
        sqlite3_free(err);
        throw std::runtime_error(msg);
    }
}

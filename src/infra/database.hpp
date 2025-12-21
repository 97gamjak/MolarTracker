#ifndef __INFRA__DATABASE_HPP__
#define __INFRA__DATABASE_HPP__

#include <string>

struct sqlite3;

class Database final
{
   public:
    explicit Database(const std::string path);
    ~Database();

    Database(const Database&)            = delete;
    Database& operator=(const Database&) = delete;

    void exec(const std::string& sql);

   private:
    sqlite3* _db = nullptr;
};

#endif   // __INFRA__DATABASE_HPP__
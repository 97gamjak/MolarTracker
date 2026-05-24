#ifndef __REPO__SRC__REPO__BASE_REPO_HPP__
#define __REPO__SRC__REPO__BASE_REPO_HPP__

#include "orm/crud.hpp"
namespace db
{
    class Database;
}   // namespace db

namespace repo
{
    /**
     * @brief A base repository class for database operations
     *
     */
    class BaseRepo
    {
       private:
        /// Reference to the database instance
        db::Database& _db;

        /// Instance of the Crud class for performing CRUD operations
        orm::Crud _crud;

       public:
        explicit BaseRepo(db::Database& database);

        ~BaseRepo()                          = default;
        BaseRepo(const BaseRepo&)            = delete;
        BaseRepo& operator=(const BaseRepo&) = delete;
        BaseRepo(BaseRepo&&)                 = delete;
        BaseRepo& operator=(BaseRepo&&)      = delete;

       protected:
        [[nodiscard]] db::Database& _getDb() const;
        [[nodiscard]] orm::Crud&    _getCrud();
    };
}   // namespace repo

#endif   // __REPO__SRC__REPO__BASE_REPO_HPP__

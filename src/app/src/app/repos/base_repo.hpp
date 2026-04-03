#ifndef __APP__SRC__APP__BASE_REPO_HPP__
#define __APP__SRC__APP__BASE_REPO_HPP__

namespace db
{
    class Database;
}   // namespace db

namespace app
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

       public:
        explicit BaseRepo(db::Database& database);

        ~BaseRepo()                          = default;
        BaseRepo(const BaseRepo&)            = delete;
        BaseRepo& operator=(const BaseRepo&) = delete;
        BaseRepo(BaseRepo&&)                 = delete;
        BaseRepo& operator=(BaseRepo&&)      = delete;

       protected:
        [[nodiscard]] db::Database& _getDb() const;
    };
}   // namespace app

#endif   // __APP__SRC__APP__BASE_REPO_HPP__

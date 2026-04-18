#include "base_repo.hpp"

namespace app
{
    /**
     * @brief Construct a new Base Repo:: Base Repo object
     *
     * @param database
     */
    BaseRepo::BaseRepo(db::Database& database) : _db(database) {}

    /**
     * @brief Gets the database instance associated with the repository.
     *
     * @return db::Database&
     */
    db::Database& BaseRepo::_getDb() const { return _db; }

    /**
     * @brief Gets the Crud instance associated with the repository.
     *
     * @return orm::Crud&
     */
    orm::Crud& BaseRepo::_getCrud() { return _crud; }
}   // namespace app

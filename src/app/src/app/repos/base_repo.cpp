#include "base_repo.hpp"

namespace app
{
    BaseRepo::BaseRepo(db::Database& database) : _db(database) {}

    db::Database& BaseRepo::_getDb() const { return _db; }
}   // namespace app

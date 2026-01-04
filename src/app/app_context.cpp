#include "app/app_context.hpp"

#include "db/database.hpp"

namespace app
{

    AppContext::AppContext(std::string dbPath)
        : _database{dbPath},
          _repos{_database},
          _services{_repos},
          _store{_services},
          _config{}
    {
    }

}   // namespace app
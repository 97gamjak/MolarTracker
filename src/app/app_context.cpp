#include "app/app_context.hpp"

#include "db/database.hpp"

namespace app
{

    AppContext::AppContext()
        : _config{},
          _database{_config.get_database_path()},
          _repos{_database},
          _services{_repos},
          _store{_services}
    {
    }
}   // namespace app

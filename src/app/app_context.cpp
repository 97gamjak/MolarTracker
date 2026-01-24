#include "app/app_context.hpp"

#include "config/constants.hpp"
#include "db/database.hpp"

namespace app
{

    AppContext::AppContext()
        : _config{Constants::getInstance().get_config_path()},
          _database{Constants::getInstance().get_database_path()},
          _repos{_database},
          _services{_repos},
          _store{_services}
    {
    }
}   // namespace app

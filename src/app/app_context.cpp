#include "app/app_context.hpp"

#include "config/constants.hpp"
#include "db/database.hpp"

namespace app
{

    AppContext::AppContext()
        : _settings{Constants::getInstance().getConfigPath()},
          _database{Constants::getInstance().getDatabasePath()},
          _repos{_database},
          _services{_repos},
          _store{_services}
    {
    }
}   // namespace app

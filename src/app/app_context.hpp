#ifndef __APP__APP_CONTEXT_HPP__
#define __APP__APP_CONTEXT_HPP__

#include <string>

#include "app/repo_container.hpp"
#include "app/service_container.hpp"
#include "app/store_container.hpp"
#include "db/database.hpp"
#include "settings/settings.hpp"

namespace app
{

    class AppContext
    {
       private:
        settings::Settings _settings;
        db::Database       _database;
        RepoContainer      _repos;
        ServiceContainer   _services;
        StoreContainer     _store;

       public:
        explicit AppContext();

        AppContext(const AppContext&)            = delete;
        AppContext& operator=(const AppContext&) = delete;

        StoreContainer&       getStore() { return _store; }
        const StoreContainer& getStore() const { return _store; }

        settings::Settings&       getSettings() { return _settings; }
        const settings::Settings& getSettings() const { return _settings; }
    };

}   // namespace app

#endif   // __APP__APP_CONTEXT_HPP__
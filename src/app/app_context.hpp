#ifndef __APP__APP_CONTEXT_HPP__
#define __APP__APP_CONTEXT_HPP__

#include <string>

#include "app/app_config.hpp"
#include "app/repo_container.hpp"
#include "app/service_container.hpp"
#include "app/store_container.hpp"
#include "db/database.hpp"

namespace app
{

    class AppContext
    {
       private:
        db::Database     _database;
        RepoContainer    _repos;
        ServiceContainer _services;
        StoreContainer   _store;
        AppConfig        _config;

       public:
        explicit AppContext(std::string dbPath);

        AppContext(const AppContext&)            = delete;
        AppContext& operator=(const AppContext&) = delete;

        StoreContainer&       getStore() noexcept { return _store; }
        const StoreContainer& getStore() const noexcept { return _store; }

        AppConfig&       getConfig() noexcept { return _config; }
        const AppConfig& getConfig() const noexcept { return _config; }
    };

}   // namespace app

#endif   // __APP__APP_CONTEXT_HPP__
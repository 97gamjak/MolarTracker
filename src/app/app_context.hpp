#ifndef __APP__APP_CONTEXT_HPP__
#define __APP__APP_CONTEXT_HPP__

#include <string>

#include "app/repo_container.hpp"
#include "app/service_container.hpp"
#include "db/database.hpp"

namespace app
{

    class AppContext
    {
       private:
        db::Database     _database;
        RepoContainer    _repos;
        ServiceContainer _services;

        void _ensureSchemaAndDefaults();

       public:
        explicit AppContext(std::string dbPath);

        AppContext(const AppContext&)            = delete;
        AppContext& operator=(const AppContext&) = delete;

        RepoContainer&       repos() noexcept;
        const RepoContainer& repos() const noexcept;

        ServiceContainer&       services() noexcept;
        const ServiceContainer& services() const noexcept;
    };

}   // namespace app

#endif   // __APP__APP_CONTEXT_HPP__
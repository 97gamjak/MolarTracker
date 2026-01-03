#include "app_context.hpp"

#include "database.hpp"

namespace app
{

    AppContext::AppContext(std::string dbPath)
        : _database{dbPath}, _repos{_database}, _services{_repos}
    {
        _ensureSchemaAndDefaults();
    }

    RepoContainer& AppContext::repos() noexcept { return _repos; }

    const RepoContainer& AppContext::repos() const noexcept { return _repos; }
    ServiceContainer&    AppContext::services() noexcept { return _services; }

    const ServiceContainer& AppContext::services() const noexcept
    {
        return _services;
    }

    void AppContext::_ensureSchemaAndDefaults()
    {
        _repos.profileRepo().ensureSchema();
        _services.profileService().ensureDefaultProfile("Default Profile");
    }

}   // namespace app
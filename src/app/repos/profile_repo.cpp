#include "profile_repo.hpp"

#include <optional>
#include <vector>

#include "database.hpp"
#include "profile.hpp"

ProfileRepo::ProfileRepo(Database& db) : _db{db} {}

void ProfileRepo::ensureSchema()
{
    _db.exec(R"sql(
        CREATE TABLE IF NOT EXISTS profiles (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE
        )
    )sql");
}

std::optional<Profile> ProfileRepo::getByID(ProfileID id) const
{
    // TODO: Implement database retrieval logic
    return std::nullopt;
}

std::vector<Profile> ProfileRepo::getAll() const
{
    // TODO: Implement database retrieval logic
    return {};
}

ProfileID ProfileRepo::create(const std::string& name)
{
    // TODO: Implement database insertion logic
    return 0;
}

void ProfileRepo::rename(ProfileID id, const std::string& newName)
{
    // TODO: Implement database update logic
}

void ProfileRepo::remove(ProfileID id)
{
    // TODO: Implement database deletion logic
}
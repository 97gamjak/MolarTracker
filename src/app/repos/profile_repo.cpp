#include "profile_repo.hpp"

#include <optional>
#include <vector>

#include "db/database.hpp"
#include "domain/profile.hpp"
#include "orm/crud.hpp"

namespace app
{

    ProfileRepo::ProfileRepo(db::Database& db) : _db{db} {}

    void ProfileRepo::ensureSchema()
    {
        _db.execute(R"sql(
        CREATE TABLE IF NOT EXISTS profiles (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE
        )
    )sql");
    }

    inline Profile ProfileRepo::toDomain(const ProfileRow& row) const
    {
        return Profile{row.id.value(), row.name.value(), row.email.value()};
    }

    inline std::vector<Profile> ProfileRepo::toDomain(
        const std::vector<ProfileRow>& rows
    ) const
    {
        std::vector<Profile> profiles;

        for (const auto& row : rows)
            profiles.push_back(toDomain(row));

        return profiles;
    }

    inline ProfileRow ProfileRepo::toRow(const Profile& profile) const
    {
        ProfileRow row;
        row.id    = profile.id();
        row.name  = profile.name();
        row.email = profile.email();
        return row;
    }

    std::optional<Profile> ProfileRepo::getByID(ProfileID id) const
    {
        return toDomain(orm::select_by_pk<ProfileRow>(_db, id));
    }

    std::vector<Profile> ProfileRepo::getAll() const
    {
        return toDomain(orm::select_all<ProfileRow>(_db));
    }

    ProfileID ProfileRepo::create(const std::string& /*name*/)
    {
        return ProfileID::invalid();
    }

    void ProfileRepo::rename(ProfileID /*id*/, const std::string& /*newName*/)
    {
        // TODO: Implement database update logic
    }

    void ProfileRepo::remove(ProfileID /*id*/)
    {
        // TODO: Implement database deletion logic
    }

}   // namespace app
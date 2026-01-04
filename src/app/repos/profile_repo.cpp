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
        row.id    = profile.getId();
        row.name  = profile.getName();
        row.email = profile.getEmail();
        return row;
    }

    std::vector<Profile> ProfileRepo::getAll() const
    {
        return toDomain(orm::get_all<ProfileRow>(_db));
    }

    std::optional<Profile> ProfileRepo::getById(ProfileId id) const
    {
        const auto profile = orm::get_by_pk<ProfileRow>(_db, id);

        if (profile.has_value())
            return toDomain(profile.value());

        return std::nullopt;
    }

    std::optional<Profile> ProfileRepo::getByName(const std::string& name) const
    {
        using name_field = decltype(ProfileRow::name);

        const auto profile = orm::get_by_unique_field<ProfileRow, name_field>(
            _db,
            name_field{name}
        );

        if (profile.has_value())
            return toDomain(profile.value());

        return std::nullopt;
    }

    ProfileId ProfileRepo::create(const std::string& /*name*/)
    {
        return ProfileId::invalid();
    }

    void ProfileRepo::rename(ProfileId /*id*/, const std::string& /*newName*/)
    {
        // TODO: Implement database update logic
    }

    void ProfileRepo::remove(ProfileId /*id*/)
    {
        // TODO: Implement database deletion logic
    }

}   // namespace app
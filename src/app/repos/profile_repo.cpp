#include "profile_repo.hpp"

#include <format>
#include <optional>
#include <vector>

#include "db/database.hpp"
#include "domain/profile.hpp"
#include "orm/crud.hpp"

namespace app
{

    /**
     * @brief Construct a new Profile Repo object
     *
     * @param db
     */
    ProfileRepo::ProfileRepo(db::Database& db) : _db{db} { ensureSchema(); }

    /**
     * @brief Ensure the database schema for profiles exists
     *
     */
    void ProfileRepo::ensureSchema() { orm::create_table<ProfileRow>(_db); }

    // TODO(97gamjak): create a static factory class to handle these conversions
    // https://97gamjak.atlassian.net/browse/MOLTRACK-86
    /**
     * @brief Convert a ProfileRow to a Profile domain object
     *
     * @param row
     * @return Profile
     */
    Profile toDomain(const ProfileRow& row)
    {
        return Profile{row.id.value(), row.name.value(), row.email.value()};
    }

    // TODO(97gamjak): create a static factory class to handle these conversions
    // https://97gamjak.atlassian.net/browse/MOLTRACK-86
    /**
     * @brief Convert a vector of ProfileRow to a vector of Profile domain
     * objects
     *
     * @param rows
     * @return std::vector<Profile>
     */
    std::vector<Profile> toDomains(const std::vector<ProfileRow>& rows)
    {
        std::vector<Profile> profiles;

        for (const auto& row : rows)
            profiles.push_back(toDomain(row));

        return profiles;
    }

    // TODO(97gamjak): create a static factory class to handle these conversions
    // https://97gamjak.atlassian.net/browse/MOLTRACK-86
    /**
     * @brief Convert a Profile domain object to a ProfileRow
     *
     * @param profile
     * @return ProfileRow
     */
    ProfileRow toRow(const Profile& profile)
    {
        ProfileRow row;
        row.id    = profile.getId();
        row.name  = profile.getName();
        row.email = profile.getEmail();
        return row;
    }

    /**
     * @brief Get all profiles from the database
     *
     * @return std::vector<Profile>
     */
    std::vector<Profile> ProfileRepo::getAll() const
    {
        return toDomains(orm::get_all<ProfileRow>(_db));
    }

    /**
     * @brief Get a profile by its ID
     *
     * @param id
     * @return std::optional<Profile>
     */
    std::optional<Profile> ProfileRepo::get(ProfileId id) const
    {
        const auto profile = orm::get_by_pk<ProfileRow>(_db, id);

        if (profile.has_value())
            return toDomain(profile.value());

        return std::nullopt;
    }

    /**
     * @brief Get a profile by its name
     *
     * @param name
     * @return std::optional<Profile>
     */
    std::optional<Profile> ProfileRepo::get(const std::string& name) const
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

    /**
     * @brief Create a new profile in the database
     *
     * @param name
     * @param email
     * @return ProfileId
     */
    ProfileId ProfileRepo::create(
        const std::string&         name,
        std::optional<std::string> email
    )
    {
        const auto profile = Profile{ProfileId::from(0), name, email};
        const auto rowId   = orm::insert(_db, toRow(profile));

        if (rowId <= 0)
        {
            // TODO: introduce MT specific error handling for repos
            // https://97gamjak.atlassian.net/browse/MOLTRACK-87
            throw std::runtime_error(
                std::format("Failed to insert new profile with name '{}'", name)
            );
        }

        const auto createdProfile = get(name);
        if (!createdProfile.has_value())
        {
            // TODO: introduce MT specific error handling for repos
            // https://97gamjak.atlassian.net/browse/MOLTRACK-87
            throw std::runtime_error(
                std::format(
                    "Failed to retrieve newly created profile with name '{}'",
                    name
                )
            );
        }

        return createdProfile->getId();
    }

    /**
     * @brief Update an existing profile in the database
     *
     * @param id
     * @param newName
     * @param newEmail
     */
    void ProfileRepo::update(
        ProfileId                         id,
        const std::string&                newName,
        const std::optional<std::string>& newEmail
    )
    {
        const auto existingProfileOpt = get(id);
        if (!existingProfileOpt.has_value())
        {
            // TODO: introduce MT specific error handling for repos
            // https://97gamjak.atlassian.net/browse/MOLTRACK-87
            throw std::runtime_error(
                std::format("Profile with ID {} not found", id.value())
            );
        }

        const auto& existingProfile = existingProfileOpt.value();
        Profile     updatedProfile{existingProfile.getId(), newName, newEmail};

        orm::update(_db, toRow(updatedProfile));
    }

    /**
     * @brief Remove a profile from the database
     *
     * @param id
     */
    void ProfileRepo::remove(ProfileId id)
    {
        orm::delete_by_pk<ProfileRow>(_db, id);
    }

}   // namespace app
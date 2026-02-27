#include "profile_repo.hpp"

#include <format>
#include <optional>
#include <vector>

#include "app/factories/profile_factory.hpp"
#include "config/logging_base.hpp"
#include "db/database.hpp"
#include "domain/profile.hpp"
#include "orm/crud.hpp"
#include "sql_models/profile_row.hpp"

#define __LOG_CATEGORY__ LogCategory::repo_profile
#include "logging/log_macros.hpp"

namespace app
{

    /**
     * @brief Construct a new Profile Repo object
     *
     * @param db
     */
    ProfileRepo::ProfileRepo(db::Database& db) : _db{db} { _ensureSchema(); }

    /**
     * @brief Ensure the database schema for profiles exists
     *
     */
    void ProfileRepo::ensureSchema() { _ensureSchema(); }

    /**
     * @brief Ensure the database schema for profiles exists, this is a private
     * helper method that is called by the virtual overriding method
     * ensureSchema() to not use virtual dispatch in constructor
     *
     */
    void ProfileRepo::_ensureSchema() { orm::create_table<ProfileRow>(_db); }

    /**
     * @brief Get all profiles from the database
     *
     * @return std::vector<Profile>
     */
    std::vector<Profile> ProfileRepo::getAll() const
    {
        return ProfileFactory::toDomains(orm::get_all<ProfileRow>(_db));
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
            return ProfileFactory::toDomain(profile.value());

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
            return ProfileFactory::toDomain(profile.value());

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
        const auto rowId   = orm::insert(_db, ProfileFactory::toRow(profile));

        if (rowId.has_value())
            return ProfileId::from(rowId.value());

        const auto msg =
            "Inserting profile with name '" + name +
            "' failed: " + rowId.error().getMessage() + " (type: " +
            orm::CrudErrorTypeMeta::toString(rowId.error().getType()) + ")";

        LOG_ERROR(msg);
        throw orm::CrudException(msg);
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
            // TODO(97gamjak): introduce MT specific error handling for repos
            // https://97gamjak.atlassian.net/browse/MOLTRACK-87
            throw std::runtime_error(
                std::format("Profile with ID {} not found", id.value())
            );
        }

        const auto& existingProfile = existingProfileOpt.value();
        Profile     updatedProfile{existingProfile.getId(), newName, newEmail};

        const auto result =
            orm::update(_db, ProfileFactory::toRow(updatedProfile));

        if (!result)
        {
            const auto msg =
                "Updating profile with ID '" + std::to_string(id.value()) +
                "' failed: " + result.error().getMessage() + " (type: " +
                orm::CrudErrorTypeMeta::toString(result.error().getType()) +
                ")";

            LOG_ERROR(msg);
            throw orm::CrudException(msg);
        }
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
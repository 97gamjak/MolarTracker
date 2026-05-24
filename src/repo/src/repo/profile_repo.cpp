#include "profile_repo.hpp"

#include <format>
#include <optional>
#include <vector>

#include "domain/profile.hpp"
#include "logging/log_macros.hpp"
#include "orm/crud.hpp"
#include "orm/crud/crud_error.hpp"
#include "orm/query_options.hpp"
#include "repo/factories/profile_factory.hpp"
#include "repo_errors.hpp"
#include "sql_models/profile_row.hpp"

REGISTER_LOG_CATEGORY("Repo.ProfileRepo");

using domain::Profile;

namespace repo
{

    /**
     * @brief Get all profiles from the database
     *
     * @return std::vector<Profile>
     */
    std::vector<Profile> ProfileRepo::getAll() const
    {
        return ProfileFactory::toDomains(orm::Crud().get<ProfileRow>(_getDb()));
    }

    /**
     * @brief Get a profile by its ID
     *
     * @param id
     * @return std::optional<Profile>
     */
    std::optional<Profile> ProfileRepo::get(ProfileId id) const
    {
        const auto query = orm::Query{}.where<ProfileRow::idField>(
            id,
            filter::Operator::Equal
        );
        const auto profile = orm::Crud().getUnique<ProfileRow>(_getDb(), query);

        if (profile.has_value())
            return ProfileFactory::toDomain(profile.value());

        LOG_INFO(
            std::format(
                "Profile with ID '{}' not found in database",
                id.value()
            )
        );

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
        const auto query = orm::Query{}.where(ProfileRow::hasName(name));

        const auto profile = orm::Crud().getUnique<ProfileRow>(_getDb(), query);

        if (profile.has_value())
            return ProfileFactory::toDomain(profile.value());

        LOG_INFO(
            std::format("Profile with name '{}' not found in database", name)
        );

        return std::nullopt;
    }

    /**
     * @brief Create a new profile in the database
     *
     * @param profile
     * @return ProfileId
     */
    ProfileId ProfileRepo::create(const domain::Profile& profile)
    {
        const auto rowId =
            orm::Crud().insert(_getDb(), ProfileFactory::toRow(profile));

        if (rowId.has_value())
            return ProfileId::from(rowId.value());

        const auto whatFailed = "profile with name '" + profile.getName() + "'";
        const auto msg        = getInsertError(rowId.error(), whatFailed);

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

        auto existingProfile = existingProfileOpt.value();
        existingProfile.setName(newName);
        existingProfile.setEmail(newEmail);

        const auto result = orm::Crud().update(
            _getDb(),
            ProfileFactory::toRow(existingProfile)
        );

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
        orm::Crud().deleteByPk(_getDb(), ProfileRow{id});
    }

}   // namespace repo
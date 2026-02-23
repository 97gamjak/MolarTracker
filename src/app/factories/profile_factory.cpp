#include "profile_factory.hpp"

#include "domain/profile.hpp"
#include "sql_models/profile_row.hpp"

namespace app
{
    /**
     * @brief Convert a ProfileRow to a Profile domain object
     *
     * @param row
     * @return Profile
     */
    Profile ProfileFactory::toDomain(const ProfileRow& row)
    {
        return Profile{row.id.value(), row.name.value(), row.email.value()};
    }

    /**
     * @brief Convert a vector of ProfileRow to a vector of Profile domain
     * objects
     *
     * @param rows
     * @return std::vector<Profile>
     */
    std::vector<Profile> ProfileFactory::toDomains(
        const std::vector<ProfileRow>& rows
    )
    {
        std::vector<Profile> profiles;
        profiles.reserve(rows.size());

        for (const auto& row : rows)
            profiles.push_back(toDomain(row));

        return profiles;
    }

    /**
     * @brief Convert a Profile domain object to a ProfileRow
     *
     * @param profile
     * @return ProfileRow
     */
    ProfileRow ProfileFactory::toRow(const Profile& profile)
    {
        ProfileRow row;

        row.id    = profile.getId();
        row.name  = profile.getName();
        row.email = profile.getEmail();

        return row;
    }

}   // namespace app
#ifndef __APP__REPOS__PROFILE_REPO_HPP__
#define __APP__REPOS__PROFILE_REPO_HPP__

#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "domain/profile.hpp"
#include "sql_models/profile_row.hpp"

namespace app
{

    class Profile;

    class IProfileRepo
    {
       public:
        virtual ~IProfileRepo() = default;

        virtual void ensureSchema() = 0;

        virtual std::vector<Profile>   getAll() const                = 0;
        virtual std::optional<Profile> get(ProfileId id) const       = 0;
        virtual std::optional<Profile> get(const std::string&) const = 0;

        virtual ProfileId create(
            const std::string&         name,
            std::optional<std::string> email
        ) = 0;

        virtual void update(
            ProfileId                         id,
            const std::string&                newName,
            const std::optional<std::string>& newEmail
        ) = 0;

        virtual void remove(ProfileId id) = 0;
    };

}   // namespace app

#endif   // __APP__REPOS__PROFILE_REPO_HPP__
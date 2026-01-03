#ifndef __APP__REPOS__PROFILE_REPO_HPP__
#define __APP__REPOS__PROFILE_REPO_HPP__

#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "domain/profile.hpp"

namespace app
{

    class Profile;

    class IProfileRepo
    {
       public:
        virtual ~IProfileRepo() = default;

        virtual void ensureSchema() = 0;

        virtual std::optional<Profile> getByID(ProfileID id) const = 0;
        virtual std::vector<Profile>   getAll() const              = 0;

        virtual ProfileID create(const std::string& name) = 0;

        virtual void rename(ProfileID id, const std::string& newName) = 0;
        virtual void remove(ProfileID id)                             = 0;
    };

}   // namespace app

#endif   // __APP__REPOS__PROFILE_REPO_HPP__
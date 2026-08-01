#ifndef __REPO__SRC__REPO__PROFILE_REPO_HPP__
#define __REPO__SRC__REPO__PROFILE_REPO_HPP__

#include <optional>
#include <vector>

#include "base_repo.hpp"
#include "repo/i_profile_repo.hpp"

namespace domain
{
    class Profile;   // Forward declaration
}   // namespace domain

namespace repo
{

    /**
     * @brief Database implementation of Profile repository
     *
     */
    class ProfileRepo : public IProfileRepo, public BaseRepo
    {
       public:
        using BaseRepo::BaseRepo;

        [[nodiscard]]
        std::vector<domain::Profile> getAll() const override;

        [[nodiscard]]
        std::optional<domain::Profile> get(ProfileId id) const override;

        [[nodiscard]]
        std::optional<domain::Profile> get(
            const std::string& name
        ) const override;

        [[nodiscard]]
        CrudResult<ProfileId> create(const domain::Profile& profile) override;

        void update(
            ProfileId                         id,
            const std::string&                newName,
            const std::optional<std::string>& newEmail
        ) override;

        void remove(ProfileId id) override;
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__PROFILE_REPO_HPP__

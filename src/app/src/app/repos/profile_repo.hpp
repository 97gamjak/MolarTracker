#ifndef __APP__SRC__APP__REPOS__PROFILE_REPO_HPP__
#define __APP__SRC__APP__REPOS__PROFILE_REPO_HPP__

#include <optional>
#include <vector>

#include "app/repos/base_repo.hpp"
#include "app/repos_api/i_profile_repo.hpp"

namespace domain
{
    class Profile;   // Forward declaration
}   // namespace domain

namespace app
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
        ProfileId create(const domain::Profile& profile) override;

        void update(
            ProfileId                         id,
            const std::string&                newName,
            const std::optional<std::string>& newEmail
        ) override;

        void remove(ProfileId id) override;
    };

}   // namespace app

#endif   // __APP__SRC__APP__REPOS__PROFILE_REPO_HPP__
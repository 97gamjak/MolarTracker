#ifndef __APP__SRC__APP__REPOS__PROFILE_REPO_HPP__
#define __APP__SRC__APP__REPOS__PROFILE_REPO_HPP__

#include <memory>
#include <optional>
#include <vector>

#include "app/repos_api/i_profile_repo.hpp"

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace app
{

    /**
     * @brief Database implementation of Profile repository
     *
     */
    class ProfileRepo : public IProfileRepo
    {
       private:
        /// reference to the database instance
        db::Database& _db;

       public:
        explicit ProfileRepo(db::Database& db);

        ~ProfileRepo() override                    = default;
        ProfileRepo(const ProfileRepo&)            = delete;
        ProfileRepo& operator=(const ProfileRepo&) = delete;
        ProfileRepo(ProfileRepo&&)                 = delete;
        ProfileRepo& operator=(ProfileRepo&&)      = delete;

        void ensureSchema() override;

        [[nodiscard]] std::vector<Profile>   getAll() const override;
        [[nodiscard]] std::optional<Profile> get(ProfileId id) const override;
        [[nodiscard]] std::optional<Profile> get(
            const std::string& name
        ) const override;

        ProfileId create(
            const std::string&         name,
            std::optional<std::string> email
        ) override;

        void update(
            ProfileId                         id,
            const std::string&                newName,
            const std::optional<std::string>& newEmail
        ) override;

        void remove(ProfileId id) override;

       private:
        void _ensureSchema() const;
    };

}   // namespace app

#endif   // __APP__SRC__APP__REPOS__PROFILE_REPO_HPP__
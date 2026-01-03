#ifndef __APP__REPOS__DB_PROFILE_REPO_HPP__
#define __APP__REPOS__DB_PROFILE_REPO_HPP__

#include "repos_api/i_profile_repo.hpp"

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace app
{

    class ProfileRepo : public IProfileRepo
    {
       private:
        db::Database& _db;

       public:
        explicit ProfileRepo(db::Database& db);

        void ensureSchema() override;

        std::optional<Profile> getByID(ProfileID id) const override;
        std::vector<Profile>   getAll() const override;

        ProfileID create(const std::string& name) override;

        void rename(ProfileID id, const std::string& newName) override;
        void remove(ProfileID id) override;
    };

}   // namespace app

#endif   // __APP__REPOS__DB_PROFILE_REPO_HPP__
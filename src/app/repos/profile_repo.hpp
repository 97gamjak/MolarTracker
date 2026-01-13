#ifndef __APP__REPOS__DB_PROFILE_REPO_HPP__
#define __APP__REPOS__DB_PROFILE_REPO_HPP__

#include <optional>
#include <vector>

#include "repos_api/i_profile_repo.hpp"
#include "sql_models/profile_row.hpp"

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

        std::vector<Profile>   getAll() const override;
        std::optional<Profile> get(ProfileId id) const override;
        std::optional<Profile> get(const std::string&) const override;

        ProfileId create(
            const std::string&         name,
            std::optional<std::string> email
        ) override;

        void rename(ProfileId id, const std::string& newName) override;
        void remove(ProfileId id) override;
    };

}   // namespace app

#endif   // __APP__REPOS__DB_PROFILE_REPO_HPP__
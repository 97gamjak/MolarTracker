#ifndef __APP__REPOS__DB_PROFILE_REPO_HPP__
#define __APP__REPOS__DB_PROFILE_REPO_HPP__

#include "i_profile_repo.hpp"

namespace db
{
    class Database;
}   // namespace db

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

#endif   // __APP__REPOS__DB_PROFILE_REPO_HPP__
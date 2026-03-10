#include "app/repo_container.hpp"

#include "db/database.hpp"
#include "repos/account_repo.hpp"
#include "repos/profile_repo.hpp"

namespace app
{

    /**
     * @brief Construct a new Repo Container object
     *
     * @param db
     */
    RepoContainer::RepoContainer(const std::shared_ptr<db::Database>& db)
        : _profileRepo{std::make_shared<ProfileRepo>(db)},
          _accountRepo{std::make_shared<AccountRepo>(db)}
    {
    }

    /**
     * @brief Get the Profile Repo
     *
     * @return std::shared_ptr<IProfileRepo>
     */
    std::shared_ptr<IProfileRepo> RepoContainer::getProfileRepo()
    {
        return _profileRepo;
    }

    /**
     * @brief Get the Profile Repo (const version)
     *
     * @return std::shared_ptr<const IProfileRepo>
     */
    std::shared_ptr<const IProfileRepo> RepoContainer::getProfileRepo() const
    {
        return _profileRepo;
    }

    /**
     * @brief Get the Account Repo
     *
     * @return std::shared_ptr<IAccountRepo>
     */
    std::shared_ptr<IAccountRepo> RepoContainer::getAccountRepo()
    {
        return _accountRepo;
    }

    /**
     * @brief Get the Account Repo (const version)
     *
     * @return std::shared_ptr<const IAccountRepo>
     */
    std::shared_ptr<const IAccountRepo> RepoContainer::getAccountRepo() const
    {
        return _accountRepo;
    }

}   // namespace app
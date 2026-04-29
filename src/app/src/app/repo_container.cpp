#include "app/repo_container.hpp"

#include "app/repos/instrument_repo.hpp"
#include "db/database.hpp"
#include "repos/account_repo.hpp"
#include "repos/profile_repo.hpp"
#include "repos/transaction_repo.hpp"

namespace app
{

    /**
     * @brief Construct a new Repo Container object
     *
     * @param db
     */
    RepoContainer::RepoContainer(db::Database& db)
        : _profileRepo{std::make_shared<ProfileRepo>(db)},
          _accountRepo{std::make_shared<AccountRepo>(db)},
          _transactionRepo{std::make_shared<TransactionRepo>(db)},
          _instrumentRepo{std::make_shared<InstrumentRepo>(db)}
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

    /**
     * @brief Get the Transaction Repo
     *
     * @return std::shared_ptr<ITransactionRepo>
     */
    std::shared_ptr<ITransactionRepo> RepoContainer::getTransactionRepo()
    {
        return _transactionRepo;
    }

    /**
     * @brief Get the Transaction Repo (const version)
     *
     * @return std::shared_ptr<const ITransactionRepo>
     */
    std::shared_ptr<const ITransactionRepo> RepoContainer::getTransactionRepo(
    ) const
    {
        return _transactionRepo;
    }

    /**
     * @brief Get the Instrument Repo
     *
     * @return std::shared_ptr<IInstrumentRepo>
     */
    std::shared_ptr<IInstrumentRepo> RepoContainer::getInstrumentRepo()
    {
        return _instrumentRepo;
    }

    /**
     * @brief Get the Instrument Repo (const version)
     *
     * @return std::shared_ptr<const IInstrumentRepo>
     */
    std::shared_ptr<const IInstrumentRepo> RepoContainer::getInstrumentRepo(
    ) const
    {
        return _instrumentRepo;
    }

}   // namespace app
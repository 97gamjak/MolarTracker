#include "repo/repo_container.hpp"

#include "account_repo.hpp"
#include "config/constants/constants.hpp"
#include "instrument_repo.hpp"
#include "logging/log_macros.hpp"
#include "position_repo.hpp"
#include "profile_repo.hpp"
#include "repo/migration/migration_runner.hpp"
#include "repo/repo_errors.hpp"
#include "transaction_repo.hpp"

REGISTER_LOG_CATEGORY("Repo.Container");

namespace repo
{

    /**
     * @brief Construct a new Repo Container object
     *
     */
    RepoContainer::RepoContainer()
        : _database{std::make_unique<db::Database>(
              Constants::getInstance().getDatabasePath()
          )},
          _migrationRunner{std::make_unique<MigrationRunner>(*_database)},
          _profileRepo{std::make_shared<ProfileRepo>(*_database)},
          _accountRepo{std::make_shared<AccountRepo>(*_database)},
          _transactionRepo{std::make_shared<TransactionRepo>(*_database)},
          _instrumentRepo{std::make_shared<InstrumentRepo>(*_database)},
          _positionRepo{std::make_shared<PositionRepo>(*_database)}
    {
        if (!_migrationRunner || !_profileRepo || !_accountRepo ||
            !_transactionRepo || !_instrumentRepo || !_positionRepo)
        {
            const auto* const msg = "Failed to initialize repository container";
            LOG_ERROR(msg);
            throw RepositoryException(msg);
        }
    }

    RepoContainer::~RepoContainer() = default;

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

    /**
     * @brief Get the Position Repo
     *
     * @return std::shared_ptr<IPositionRepo>
     */
    std::shared_ptr<IPositionRepo> RepoContainer::getPositionRepo()
    {
        return _positionRepo;
    }

    /**
     * @brief Get the Position Repo (const version)
     *
     * @return std::shared_ptr<const IPositionRepo>
     */
    std::shared_ptr<const IPositionRepo> RepoContainer::getPositionRepo() const
    {
        return _positionRepo;
    }

}   // namespace repo
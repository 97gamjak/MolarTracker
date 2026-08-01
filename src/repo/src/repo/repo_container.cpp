#include "repo/repo_container.hpp"

#include "account_repo.hpp"
#include "config/constants/constants.hpp"
#include "db/backup_manager.hpp"
#include "instrument_repo.hpp"
#include "logging/log_macros.hpp"
#include "position_repo.hpp"
#include "profile_repo.hpp"
#include "repo/migration/migration_runner.hpp"
#include "repo/repo_errors.hpp"
#include "settings/backup_settings.hpp"
#include "transaction_repo.hpp"
#include "watchlist_repo.hpp"

REGISTER_LOG_CATEGORY("Repo.Container");

namespace repo
{

    /**
     * @brief Construct a new Repo Container object
     *
     * @param backupSettings The backup settings to use for creating a backup on
     *
     */
    RepoContainer::RepoContainer(const settings::BackupSettings& backupSettings)
        : _database{std::make_unique<db::Database>(
              Constants::getInstance().getDatabasePath()
          )}
    {
        try
        {
            db::BackupManager::createBackup(*_database, backupSettings);
        }
        catch (const std::exception& e)
        {
            LOG_WARNING(
                std::string{"Startup backup failed (continuing): "} + e.what()
            );
        }

        // Attention: the MigrationRunner does already apply migrations to the
        // database in its constructor, so it must be created after the database
        // is opened.
        _migrationRunner = std::make_unique<MigrationRunner>(*_database);
        _profileRepo     = std::make_shared<ProfileRepo>(*_database);
        _accountRepo     = std::make_shared<AccountRepo>(*_database);
        _transactionRepo = std::make_shared<TransactionRepo>(*_database);
        _instrumentRepo  = std::make_shared<InstrumentRepo>(*_database);
        _positionRepo    = std::make_shared<PositionRepo>(*_database);
        _watchlistRepo   = std::make_shared<WatchlistRepo>(*_database);

        if (!_migrationRunner || !_profileRepo || !_accountRepo ||
            !_transactionRepo || !_instrumentRepo || !_positionRepo ||
            !_watchlistRepo)
        {
            const auto* const msg = "Failed to initialize repository container";
            LOG_ERROR(msg);
            throw RepositoryException(msg);
        }
    }

    /**
     * @brief Close the underlying database connection.
     */
    void RepoContainer::closeDb() { _database->close(); }

    /**
     * @brief Reopen the database connection at the original path.
     *
     * @return DatabaseResult<void> Returns a DatabaseResult indicating success
     * or failure.
     */
    DatabaseResult<void> RepoContainer::reopenDb()
    {
        return _database->open(
            Constants::getInstance().getDatabasePath().string()
        );
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

    /**
     * @brief Get the Watchlist Repo
     *
     * @return std::shared_ptr<IWatchlistRepo>
     */
    std::shared_ptr<IWatchlistRepo> RepoContainer::getWatchlistRepo()
    {
        return _watchlistRepo;
    }

    /**
     * @brief Get the Watchlist Repo (const version)
     *
     * @return std::shared_ptr<const IWatchlistRepo>
     */
    std::shared_ptr<const IWatchlistRepo> RepoContainer::getWatchlistRepo(
    ) const
    {
        return _watchlistRepo;
    }

}   // namespace repo

#include "service/service_container.hpp"

#include "account_service.hpp"
#include "instrument_service.hpp"
#include "logging/log_macros.hpp"
#include "position_service.hpp"
#include "profile_service.hpp"
#include "repo/exceptions.hpp"
#include "repo/repo_container.hpp"
#include "transaction_service.hpp"
#include "watchlist_service.hpp"

REGISTER_LOG_CATEGORY("Service.ServiceContainer");

namespace service
{

    /**
     * @brief Construct a new Service Container object
     *
     * @param backupSettings The backup settings to use for creating a backup on
     *
     */
    ServiceContainer::ServiceContainer(
        const settings::BackupSettings& backupSettings
    )
    try
        : _repoContainer{std::make_unique<repo::RepoContainer>(backupSettings)},
          _profileService{
              std::make_shared<ProfileService>(_repoContainer->getProfileRepo()
              )},
          _accountService{
              std::make_shared<AccountService>(_repoContainer->getAccountRepo()
              )},
          _transactionService{std::make_shared<TransactionService>(
              _repoContainer->getTransactionRepo()
          )},
          _instrumentService{std::make_shared<InstrumentService>(
              _repoContainer->getInstrumentRepo()
          )},
          _positionService{std::make_shared<PositionService>(
              _repoContainer->getPositionRepo()
          )},
          _watchlistService{std::make_shared<WatchlistService>(
              _repoContainer->getWatchlistRepo()
          )}

    {
    }
    catch (const repo::MigrationException& e)
    {
        LOG_ERROR("Database migration failed: " + std::string(e.what()));
        throw;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR(
            "Service container initialization failed: " + std::string(e.what())
        );
        throw;
    }

    ServiceContainer::~ServiceContainer() = default;

    /**
     * @brief Get the Profile Service
     *
     * @return std::shared_ptr<IProfileService>
     */
    std::shared_ptr<IProfileService> ServiceContainer::getProfileService()
    {
        return _profileService;
    }

    /**
     * @brief Get the Profile Service (const version)
     *
     * @return std::shared_ptr<const IProfileService>
     */
    std::shared_ptr<const IProfileService> ServiceContainer::getProfileService(
    ) const
    {
        return _profileService;
    }

    /**
     * @brief Get the Account Service
     *
     * @return std::shared_ptr<IAccountService>
     */
    std::shared_ptr<IAccountService> ServiceContainer::getAccountService()
    {
        return _accountService;
    }

    /**
     * @brief Get the Account Service (const version)
     *
     * @return std::shared_ptr<const IAccountService>
     */
    std::shared_ptr<const IAccountService> ServiceContainer::getAccountService(
    ) const
    {
        return _accountService;
    }

    /**
     * @brief Get the Transaction Service
     *
     * @return std::shared_ptr<ITransactionService>
     */
    std::shared_ptr<ITransactionService> ServiceContainer::
        getTransactionService()
    {
        return _transactionService;
    }

    /**
     * @brief Get the Transaction Service (const version)
     *
     * @return std::shared_ptr<const ITransactionService>
     */
    std::shared_ptr<const ITransactionService> ServiceContainer::
        getTransactionService() const
    {
        return _transactionService;
    }

    /**
     * @brief Get the Instrument Service
     *
     * @return std::shared_ptr<IInstrumentService>
     */
    std::shared_ptr<IInstrumentService> ServiceContainer::getInstrumentService()
    {
        return _instrumentService;
    }

    /**
     * @brief Get the Instrument Service (const version)
     *
     * @return std::shared_ptr<const IInstrumentService>
     */
    std::shared_ptr<const IInstrumentService> ServiceContainer::
        getInstrumentService() const
    {
        return _instrumentService;
    }

    /**
     * @brief Get the Position Service
     *
     * @return std::shared_ptr<IPositionService>
     */
    std::shared_ptr<IPositionService> ServiceContainer::getPositionService()
    {
        return _positionService;
    }

    /**
     * @brief Get the Position Service (const version)
     *
     * @return std::shared_ptr<const IPositionService>
     */
    std::shared_ptr<const IPositionService> ServiceContainer::
        getPositionService() const
    {
        return _positionService;
    }

    /**
     * @brief Get the Watchlist Service
     *
     * @return std::shared_ptr<IWatchlistService>
     */
    std::shared_ptr<IWatchlistService> ServiceContainer::getWatchlistService()
    {
        return _watchlistService;
    }

    /**
     * @brief Get the Watchlist Service (const version)
     *
     * @return std::shared_ptr<const IWatchlistService>
     */
    std::shared_ptr<const IWatchlistService> ServiceContainer::
        getWatchlistService() const
    {
        return _watchlistService;
    }

    /**
     * @brief Close the underlying database connection.
     */
    void ServiceContainer::closeDb() { _repoContainer->closeDb(); }

    /**
     * @brief Reopen the database connection.
     *
     * @return DatabaseResult<void> Returns a DatabaseResult indicating success
     * or failure.
     */
    DatabaseResult<void> ServiceContainer::reopenDb()
    {
        return _repoContainer->reopenDb();
    }

}   // namespace service

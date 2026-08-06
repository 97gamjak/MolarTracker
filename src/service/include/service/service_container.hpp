#ifndef __SERVICE__INCLUDE__SERVICE__SERVICE_CONTAINER_HPP__
#define __SERVICE__INCLUDE__SERVICE__SERVICE_CONTAINER_HPP__

#include <memory>

#include "error/crud_error.hpp"

namespace repo
{
    class RepoContainer;   // Forward declaration
}   // namespace repo

namespace settings
{
    class BackupSettings;   // Forward declaration
}   // namespace settings

namespace service
{

    class IProfileService;        // Forward declaration
    class IAccountService;        // Forward declaration
    class ITransactionService;    // Forward declaration
    class IInstrumentService;     // Forward declaration
    class IPositionService;       // Forward declaration
    class IWatchlistService;      // Forward declaration
    class IMigrationLogService;   // Forward declaration

    /**
     * @brief Container for all services
     *
     */
    class ServiceContainer
    {
       private:
        /// The Repository container
        std::unique_ptr<repo::RepoContainer> _repoContainer;
        /// The Profile service
        std::shared_ptr<IProfileService> _profileService;
        /// The Account service
        std::shared_ptr<IAccountService> _accountService;
        /// The Transaction service
        std::shared_ptr<ITransactionService> _transactionService;
        /// The Instrument service
        std::shared_ptr<IInstrumentService> _instrumentService;
        /// The Position service
        std::shared_ptr<IPositionService> _positionService;
        /// The Watchlist service
        std::shared_ptr<IWatchlistService> _watchlistService;
        /// The migration log service
        std::shared_ptr<IMigrationLogService> _migrationLogService;

       public:
        explicit ServiceContainer(
            const settings::BackupSettings& backupSettings
        );
        ~ServiceContainer();

        [[nodiscard]] std::shared_ptr<IProfileService> getProfileService();
        [[nodiscard]] std::shared_ptr<const IProfileService> getProfileService() const;

        [[nodiscard]] std::shared_ptr<IAccountService> getAccountService();
        [[nodiscard]] std::shared_ptr<const IAccountService> getAccountService() const;

        [[nodiscard]] std::shared_ptr<ITransactionService> getTransactionService();
        [[nodiscard]] std::shared_ptr<const ITransactionService> getTransactionService() const;

        [[nodiscard]] std::shared_ptr<IInstrumentService> getInstrumentService();
        [[nodiscard]] std::shared_ptr<const IInstrumentService> getInstrumentService() const;

        [[nodiscard]] std::shared_ptr<IPositionService> getPositionService();
        [[nodiscard]] std::shared_ptr<const IPositionService> getPositionService() const;

        [[nodiscard]] std::shared_ptr<IWatchlistService> getWatchlistService();
        [[nodiscard]] std::shared_ptr<const IWatchlistService> getWatchlistService() const;

        [[nodiscard]] std::shared_ptr<IMigrationLogService> getMigrationLogService();
        [[nodiscard]] std::shared_ptr<const IMigrationLogService> getMigrationLogService() const;

        void                               closeDb();
        [[nodiscard]] DatabaseResult<void> reopenDb();
    };

}   // namespace service

#endif   // __SERVICE__INCLUDE__SERVICE__SERVICE_CONTAINER_HPP__

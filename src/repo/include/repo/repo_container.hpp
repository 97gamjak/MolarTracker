#ifndef __REPO__INCLUDE__REPO__REPO_CONTAINER_HPP__
#define __REPO__INCLUDE__REPO__REPO_CONTAINER_HPP__

#include <memory>

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace settings
{
    class BackupSettings;   // Forward declaration
}   // namespace settings

namespace repo
{

    class IProfileRepo;       // Forward declaration
    class IAccountRepo;       // Forward declaration
    class ITransactionRepo;   // Forward declaration
    class IInstrumentRepo;    // Forward declaration
    class IPositionRepo;      // Forward declaration
    class MigrationRunner;    // Forward declaration

    /**
     * @brief Container for all repositories
     *
     */
    class RepoContainer
    {
       private:
        /// The database instance for the application
        std::unique_ptr<db::Database> _database;

        /// The migration runner for the application
        std::unique_ptr<MigrationRunner> _migrationRunner;

        /// The Profile repository
        std::shared_ptr<IProfileRepo> _profileRepo;
        /// The Account repository
        std::shared_ptr<IAccountRepo> _accountRepo;
        /// The Transaction repository
        std::shared_ptr<ITransactionRepo> _transactionRepo;
        /// The Instrument repository
        std::shared_ptr<IInstrumentRepo> _instrumentRepo;
        /// The Position repository
        std::shared_ptr<IPositionRepo> _positionRepo;

       public:
        explicit RepoContainer(const settings::BackupSettings& backupSettings);
        ~RepoContainer();

        [[nodiscard]] std::shared_ptr<IProfileRepo>       getProfileRepo();
        [[nodiscard]] std::shared_ptr<const IProfileRepo> getProfileRepo(
        ) const;

        [[nodiscard]] std::shared_ptr<IAccountRepo>       getAccountRepo();
        [[nodiscard]] std::shared_ptr<const IAccountRepo> getAccountRepo(
        ) const;

        [[nodiscard]] std::shared_ptr<ITransactionRepo> getTransactionRepo();
        [[nodiscard]] std::shared_ptr<const ITransactionRepo> getTransactionRepo(
        ) const;

        [[nodiscard]] std::shared_ptr<IInstrumentRepo> getInstrumentRepo();
        [[nodiscard]] std::shared_ptr<const IInstrumentRepo> getInstrumentRepo(
        ) const;

        [[nodiscard]] std::shared_ptr<IPositionRepo>       getPositionRepo();
        [[nodiscard]] std::shared_ptr<const IPositionRepo> getPositionRepo(
        ) const;

        void closeDb();
        void reopenDb();
    };

}   // namespace repo

#endif   // __REPO__INCLUDE__REPO__REPO_CONTAINER_HPP__
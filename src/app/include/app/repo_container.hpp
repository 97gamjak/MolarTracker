#ifndef __APP__INCLUDE__APP__REPO_CONTAINER_HPP__
#define __APP__INCLUDE__APP__REPO_CONTAINER_HPP__

#include <memory>

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace app
{

    class IProfileRepo;       // Forward declaration
    class IAccountRepo;       // Forward declaration
    class ITransactionRepo;   // Forward declaration

    /**
     * @brief Container for all repositories
     *
     */
    class RepoContainer
    {
       private:
        /// The Profile repository
        std::shared_ptr<IProfileRepo> _profileRepo;
        /// The Account repository
        std::shared_ptr<IAccountRepo> _accountRepo;
        /// The Transaction repository
        std::shared_ptr<ITransactionRepo> _transactionRepo;

       public:
        explicit RepoContainer(db::Database& db);

        [[nodiscard]] std::shared_ptr<IProfileRepo>       getProfileRepo();
        [[nodiscard]] std::shared_ptr<const IProfileRepo> getProfileRepo(
        ) const;

        [[nodiscard]] std::shared_ptr<IAccountRepo>       getAccountRepo();
        [[nodiscard]] std::shared_ptr<const IAccountRepo> getAccountRepo(
        ) const;

        [[nodiscard]] std::shared_ptr<ITransactionRepo> getTransactionRepo();
        [[nodiscard]] std::shared_ptr<const ITransactionRepo> getTransactionRepo(
        ) const;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__REPO_CONTAINER_HPP__
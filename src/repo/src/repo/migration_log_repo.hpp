#ifndef __REPO__SRC__REPO__MIGRATION_LOG_REPO_HPP__
#define __REPO__SRC__REPO__MIGRATION_LOG_REPO_HPP__

#include "base_repo.hpp"
#include "repo/i_migration_log_repo.hpp"

namespace repo
{

    /**
     * @brief Database implementation of the migration log repository
     *
     */
    class MigrationLogRepo : public IMigrationLogRepo, public BaseRepo
    {
       public:
        using BaseRepo::BaseRepo;

        [[nodiscard]]
        std::vector<MigrationLogEntry> getAll() override;

        [[nodiscard]]
        std::int64_t getCurrentDbVersion() override;

        [[nodiscard]]
        std::int64_t getTargetDbVersion() override;
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__MIGRATION_LOG_REPO_HPP__

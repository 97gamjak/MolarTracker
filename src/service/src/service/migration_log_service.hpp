#ifndef __SERVICE__SRC__SERVICE__MIGRATION_LOG_SERVICE_HPP__
#define __SERVICE__SRC__SERVICE__MIGRATION_LOG_SERVICE_HPP__

#include <memory>

#include "service/i_migration_log_service.hpp"

namespace repo
{
    class IMigrationLogRepo;   // forward declaration
}   // namespace repo

namespace service
{
    /**
     * @brief Implementation of the migration log service
     *
     */
    class MigrationLogService : public IMigrationLogService
    {
       private:
        /// reference to the migration log repository
        std::shared_ptr<repo::IMigrationLogRepo> _migrationLogRepo;

       public:
        explicit MigrationLogService(
            const std::shared_ptr<repo::IMigrationLogRepo>& migrationLogRepo
        );

        [[nodiscard]]
        std::vector<MigrationLogEntry> getAll() const override;

        [[nodiscard]]
        std::int64_t getCurrentDbVersion() const override;

        [[nodiscard]]
        std::int64_t getTargetDbVersion() const override;
    };

}   // namespace service

#endif   // __SERVICE__SRC__SERVICE__MIGRATION_LOG_SERVICE_HPP__

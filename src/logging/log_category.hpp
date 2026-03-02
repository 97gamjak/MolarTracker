#ifndef __LOGGING__LOG_CATEGORY_HPP__
#define __LOGGING__LOG_CATEGORY_HPP__

#include <memory>
#include <string>
#include <vector>

#include "config/logging_base.hpp"

namespace logging
{
    using LogCategoryId                                 = std::int64_t;
    inline constexpr LogCategoryId InvalidLogCategoryId = -1;
    inline constexpr LogCategoryId RootLogCategoryId    = 0;

    /**
     * @brief helper struct to register log categories at during build step
     *
     */
    class LogCategory
    {
       private:
        LogCategoryId              _id{InvalidLogCategoryId};
        LogCategoryId              _parentId{InvalidLogCategoryId};
        std::string                _segment;
        std::string                _fullName;
        LogLevel                   _logLevel;
        std::vector<LogCategoryId> _subCategoryIds;

       public:
        static LogCategory createRootCategory(LogLevel logLevel);

        explicit LogCategory(
            LogCategoryId id,
            LogCategoryId parentId,
            std::string   segment,
            std::string   fullName,
            LogLevel      logLevel
        );

        void addSubCategory(LogCategoryId subCategoryId);
        void setLogLevel(const LogLevel& logLevel);
    };
}   // namespace logging

#endif   // __LOGGING__LOG_CATEGORY_HPP__
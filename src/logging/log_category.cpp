#include "log_category.hpp"

namespace logging
{
    LogCategory LogCategory::createRootCategory(LogLevel logLevel)
    {
        return LogCategory(
            RootLogCategoryId,
            InvalidLogCategoryId,
            "",
            "",
            logLevel
        );
    }

    LogCategory::LogCategory(
        LogCategoryId id,
        LogCategoryId parentId,
        std::string   segment,
        std::string   fullName,
        LogLevel      logLevel
    )
        : _id(id),
          _parentId(parentId),
          _segment(std::move(segment)),
          _fullName(std::move(fullName)),
          _logLevel(logLevel)
    {
    }

    void LogCategory::addSubCategory(LogCategoryId subCategoryId)
    {
        _subCategoryIds.push_back(subCategoryId);
    }

    void LogCategory::setLogLevel(const LogLevel& logLevel)
    {
        _logLevel = logLevel;
    }

}   // namespace logging
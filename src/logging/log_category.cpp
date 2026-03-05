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
        _children.push_back(subCategoryId);
    }

    void LogCategory::setLogLevel(const LogLevel& logLevel)
    {
        _logLevel = logLevel;
    }

    LogLevel LogCategory::getLogLevel() const { return _logLevel; }

    std::string LogCategory::getName() const { return _fullName; }
    std::string LogCategory::getSegment() const { return _segment; }

    LogCategoryId LogCategory::getId() const { return _id; }

    LogCategoryId LogCategory::getParentId() const { return _parentId; }

    std::vector<LogCategoryId> LogCategory::getChildren() const
    {
        return _children;
    }

    bool LogCategory::hasChildren() const { return !_children.empty(); }

}   // namespace logging
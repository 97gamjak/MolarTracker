#include "log_category.hpp"

namespace logging
{
    /**
     * @brief Construct a new Log Category:: Log Category object
     *
     * @param id The unique identifier for the log category
     * @param parentId The identifier of the parent log category, or
     * InvalidLogCategoryId if this is a root category
     * @param segment The segment name of the log category (the last part of the
     * full name)
     * @param fullName The full name of the log category, including all segments
     * separated by dots
     * @param logLevel The log level associated with this category
     */
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

    /**
     * @brief Construct a new Log Category:: Log Category object
     *
     * @param id The unique identifier for the log category
     * @param parentId The identifier of the parent log category, or
     * InvalidLogCategoryId if this is a root category
     * @param segment The segment name of the log category (the last part of the
     * full name)
     * @param fullName The full name of the log category, including all segments
     * separated by dots
     * @param logLevel The log level associated with this category
     */
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

    /**
     * @brief Add a subcategory to this log category
     *
     * @param subCategoryId The identifier of the subcategory to add
     */
    void LogCategory::addSubCategory(LogCategoryId subCategoryId)
    {
        _children.push_back(subCategoryId);
    }

    /**
     * @brief Set the log level for this category
     *
     * @param logLevel The new log level to set for this category
     */
    void LogCategory::setLogLevel(const LogLevel& logLevel)
    {
        _logLevel = logLevel;
    }

    LogLevel LogCategory::getLogLevel() const { return _logLevel; }

    /**
     * @brief Get the full name of the log category, including all segments
     * separated by dots
     *
     * @return std::string The full name of the log category, including all
     * segments separated by dots
     */
    std::string LogCategory::getName() const { return _fullName; }

    /**
     * @brief Get the segment name of the log category (the last part of the
     * full name)
     *
     * @return std::string The segment name of the log category (the last part
     * of the full name)
     */
    std::string LogCategory::getSegment() const { return _segment; }

    /**
     * @brief Get the unique identifier for the log category
     *
     * @return LogCategoryId The unique identifier for the log category
     */
    LogCategoryId LogCategory::getId() const { return _id; }

    /**
     * @brief Get the identifier of the parent log category, or
     * InvalidLogCategoryId if this is a root category
     *
     * @return LogCategoryId The identifier of the parent log category, or
     * InvalidLogCategoryId if this is a root category
     */
    LogCategoryId LogCategory::getParentId() const { return _parentId; }

    /**
     * @brief Get the identifiers of the child log categories
     *
     * @return std::vector<LogCategoryId> The identifiers of the child log
     * categories
     */
    std::vector<LogCategoryId> LogCategory::getChildren() const
    {
        return _children;
    }

    /**
     * @brief Check if this log category has any child categories
     *
     * @return true If this log category has one or more child categories
     * @return false If this log category has no child categories
     */
    bool LogCategory::hasChildren() const { return !_children.empty(); }

}   // namespace logging
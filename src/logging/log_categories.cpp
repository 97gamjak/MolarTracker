#include "log_categories.hpp"

#include <cassert>
#include <iostream>

namespace logging
{
    LogCategories::LogCategories(
        const std::set<std::string>& categoryNames,
        const LogLevel&              defaultLogLevel
    )
    {
        assert(_addLogCategory("", defaultLogLevel) != InvalidLogCategoryId);

        for (const auto& categoryName : categoryNames)
        {
            assert(
                _addLogCategory(categoryName, defaultLogLevel) !=
                InvalidLogCategoryId
            );
        }
    }

    LogCategoryId LogCategories::findLogCategory(
        const std::string& categoryName
    ) const
    {
        auto it = _categoryNameToIdMap.find(categoryName);
        if (it != _categoryNameToIdMap.end())
            return it->second;

        return InvalidLogCategoryId;
    }

    std::vector<LogCategory> LogCategories::getCategories() const
    {
        return _categories;
    }

    std::optional<LogCategory> LogCategories::getCategory(
        const LogCategoryId& categoryId
    ) const
    {
        if (categoryId == InvalidLogCategoryId ||
            categoryId >= _categories.size())
            return std::nullopt;

        return _categories[categoryId];
    }

    std::optional<LogCategory> LogCategories::getCategory(
        const std::string& categoryName
    ) const
    {
        const auto id = findLogCategory(categoryName);

        if (id == InvalidLogCategoryId)
            return std::nullopt;

        return _categories[id];
    }

    std::vector<LogCategoryId> LogCategories::getChildrenOf(
        LogCategoryId parentId
    ) const
    {
        if (parentId == InvalidLogCategoryId || parentId >= _categories.size())
            return {};

        return _categories[parentId].getSubCategoryIds();
    }

    void LogCategories::setLogLevel(
        const std::string& categoryName,
        const LogLevel&    logLevel
    )
    {
        const auto id = findLogCategory(categoryName);

        if (id == InvalidLogCategoryId)
            return;

        _categories[id].setLogLevel(logLevel);
    }

    LogCategoryId LogCategories::_getOrCreateLogCategory(
        LogCategoryId      parentCategoryId,
        const std::string& segment,
        const std::string& fullName,
        const LogLevel&    logLevel
    )
    {
        std::string fullNameCopy = fullName;

        _appendSegmentToFullName(fullNameCopy, segment);

        const auto existingId = findLogCategory(fullNameCopy);

        if (existingId != InvalidLogCategoryId)
            return existingId;

        return _addLogCategory(
            parentCategoryId,
            segment,
            fullNameCopy,
            logLevel
        );
    }

    LogCategoryId LogCategories::_addLogCategory(
        const std::string& fullName,
        const LogLevel&    logLevel
    )
    {
        if (fullName.empty())
            return RootLogCategoryId;

        const auto existingId = findLogCategory(fullName);

        if (existingId != InvalidLogCategoryId)
            return existingId;

        LogCategoryId parentId = RootLogCategoryId;
        std::string   segment;
        segment.reserve(fullName.size());

        _forEachSegment(
            fullName,
            [&](const std::string& seg)
            {
                parentId =
                    _getOrCreateLogCategory(parentId, seg, fullName, logLevel);
            }
        );

        return parentId;
    }

    LogCategoryId LogCategories::_addLogCategory(
        const LogCategoryId& parentCategoryId,
        const std::string&   segment,
        const std::string&   fullName,
        const LogLevel&      logLevel
    )
    {
        const auto newId = static_cast<LogCategoryId>(_categories.size());
        _categories
            .emplace_back(newId, parentCategoryId, segment, fullName, logLevel);
        _categoryNameToIdMap[fullName] = newId;
        _categories[parentCategoryId].addSubCategory(newId);
        return newId;
    }

    void LogCategories::_appendSegmentToFullName(
        std::string&       parentFullName,
        const std::string& segment
    )
    {
        if (!parentFullName.empty())
            parentFullName += ".";

        parentFullName += segment;
    }
}   // namespace logging
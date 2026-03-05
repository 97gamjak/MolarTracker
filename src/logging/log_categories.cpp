#include "log_categories.hpp"

#include <cassert>

#include "log_macros.hpp"

REGISTER_LOG_CATEGORY("logging.categories");

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

    LogCategories LogCategories::copy() const
    {
        LogCategories copy;
        copy._categories          = _categories;
        copy._categoryNameToIdMap = _categoryNameToIdMap;
        return copy;
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

        return _categories[parentId].getChildren();
    }

    std::vector<LogCategoryId> LogCategories::getAllDescendantsOf(
        LogCategoryId categoryId
    ) const
    {
        std::vector<LogCategoryId> descendants;
        std::vector<LogCategoryId> stack{categoryId};

        while (!stack.empty())
        {
            const auto currentId = stack.back();
            stack.pop_back();

            descendants.push_back(currentId);

            const auto children = getChildrenOf(currentId);
            stack.insert(stack.end(), children.begin(), children.end());
        }

        return descendants;
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
        {
            return _addLogCategory(
                InvalidLogCategoryId,
                "",
                fullName,
                logLevel
            );
        }

        const auto existingId = findLogCategory(fullName);

        if (existingId != InvalidLogCategoryId)
            return existingId;

        LogCategoryId parentId = RootLogCategoryId;
        LogCategoryId newId    = InvalidLogCategoryId;
        std::string   segment;
        segment.reserve(fullName.size());

        _forEachSegment(
            fullName,
            [&](const std::string& seg)
            {
                newId =
                    _getOrCreateLogCategory(parentId, seg, fullName, logLevel);
                parentId = newId;
            }
        );

        return newId;
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

        if (parentCategoryId != InvalidLogCategoryId)
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
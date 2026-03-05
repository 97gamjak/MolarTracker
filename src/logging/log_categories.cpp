#include "log_categories.hpp"

#include <algorithm>
#include <cassert>

#include "log_exceptions.hpp"
#include "log_macros.hpp"

REGISTER_LOG_CATEGORY("Logging.Categories");

namespace logging
{
    /**
     * @brief Construct a new Log Categories:: Log Categories object
     *
     * @param categoryNames
     * @param defaultLogLevel
     */
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

    /**
     * @brief Create a copy of the LogCategories object
     *
     * @return LogCategories
     */
    LogCategories LogCategories::copy() const
    {
        LogCategories copy;
        copy._categories          = _categories;
        copy._categoryNameToIdMap = _categoryNameToIdMap;
        return copy;
    }

    /**
     * @brief Find the ID of a log category by its name
     *
     * @param categoryName
     * @return LogCategoryId
     */
    LogCategoryId LogCategories::findLogCategory(
        const std::string& categoryName
    ) const
    {
        auto it = _categoryNameToIdMap.find(categoryName);

        if (it != _categoryNameToIdMap.end())
            return it->second;

        return InvalidLogCategoryId;
    }

    /**
     * @brief Get all log categories
     *
     * @return std::vector<LogCategory>
     */
    std::vector<LogCategory> LogCategories::getCategories() const
    {
        return _categories;
    }

    /**
     * @brief Get a log category by its ID
     *
     * @param categoryId
     * @return std::optional<LogCategory>
     */
    std::optional<LogCategory> LogCategories::getCategory(
        const LogCategoryId& categoryId
    ) const
    {
        if (categoryId == InvalidLogCategoryId ||
            categoryId >= _categories.size())
            return std::nullopt;

        const auto& category = _categories[categoryId];

        if (category.getId() != categoryId ||
            _categoryNameToIdMap.find(category.getName())->second != categoryId)
        {
            throw LogException(
                "Inconsistent log category data: ID and name mapping do not "
                "match"
            );
        }

        return category;
    }

    /**
     * @brief Get a log category by its name
     *
     * @param categoryName
     * @return std::optional<LogCategory>
     */
    std::optional<LogCategory> LogCategories::getCategory(
        const std::string& categoryName
    ) const
    {
        const auto id = findLogCategory(categoryName);

        if (id == InvalidLogCategoryId)
            return std::nullopt;

        return _categories[id];
    }

    /**
     * @brief Get the child categories of a given parent category
     *
     * @param parentId
     * @return std::vector<LogCategoryId>
     */
    std::vector<LogCategoryId> LogCategories::getChildrenOf(
        const LogCategoryId& parentId
    ) const
    {
        if (parentId == InvalidLogCategoryId || parentId >= _categories.size())
            return {};

        // sort children by name to ensure consistent order
        auto children = _categories[parentId].getChildren();

        std::ranges::sort(
            children,
            [this](const LogCategoryId& catA, const LogCategoryId& catB)
            {
                const auto& categoryA = _categories[catA];
                const auto& categoryB = _categories[catB];
                return categoryA.getName() < categoryB.getName();
            }
        );

        return children;
    }

    /**
     * @brief Get all descendant categories of a given category
     *
     * @param categoryId
     * @return std::vector<LogCategoryId>
     */
    std::vector<LogCategoryId> LogCategories::getAllDescendantsOf(
        const LogCategoryId& categoryId
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

    /**
     * @brief Set the log level for a given category
     *
     * @param categoryName
     * @param logLevel
     */
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

    /**
     * @brief Get or create a log category
     *
     * @param parentCategoryId
     * @param segment
     * @param fullName
     * @param logLevel
     * @return LogCategoryId
     */
    LogCategoryId LogCategories::_getOrCreateLogCategory(
        const LogCategoryId& parentCategoryId,
        const std::string&   segment,
        const std::string&   fullName,
        const LogLevel&      logLevel
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

    /**
     * @brief Add a log category by its full name
     *
     * @param fullName
     * @param logLevel
     * @return LogCategoryId
     */
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
        std::string   currentName;

        _forEachSegment(
            fullName,
            [&](const std::string& seg)
            {
                _appendSegmentToFullName(
                    currentName,
                    seg
                );   // grows: "App" → "App.Store" → ...

                const auto existingSegId = findLogCategory(currentName);
                if (existingSegId != InvalidLogCategoryId)
                {
                    // Node already exists, just move down
                    newId    = existingSegId;
                    parentId = newId;
                    return;
                }

                newId = _addLogCategory(parentId, seg, currentName, logLevel);
                parentId = newId;
            }
        );

        return newId;
    }

    /**
     * @brief Get a log category by its ID
     *
     * @param categoryId
     * @return std::optional<LogCategory>
     */
    LogCategoryId LogCategories::_addLogCategory(
        const LogCategoryId& parentCategoryId,
        const std::string&   segment,
        const std::string&   fullName,
        const LogLevel&      logLevel
    )
    {
        assert(
            fullName.size() >= segment.size() &&
            fullName.substr(fullName.size() - segment.size()) == segment
        );

        const auto newId = static_cast<LogCategoryId>(_categories.size());
        _categories
            .emplace_back(newId, parentCategoryId, segment, fullName, logLevel);
        _categoryNameToIdMap[fullName] = newId;

        if (parentCategoryId != InvalidLogCategoryId)
            _categories[parentCategoryId].addSubCategory(newId);

        return newId;
    }

    /**
     * @brief Append a segment to the full name of a log category
     *
     * @param parentFullName The full name of the parent category
     * @param segment The segment to append
     */
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
#ifndef __LOGGING__INCLUDE__LOGGING__LOG_CATEGORIES_HPP__
#define __LOGGING__INCLUDE__LOGGING__LOG_CATEGORIES_HPP__

#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "log_category.hpp"

namespace logging
{
    /**
     * @brief Class representing a collection of log categories, organized in a
     * hierarchical structure.
     *
     */
    class LogCategories
    {
       private:
        /// A vector of log categories, where each category has a unique ID
        std::vector<LogCategory> _categories;

        /// A map from category names to their corresponding IDs for quick
        /// lookup
        std::unordered_map<std::string, LogCategoryId> _categoryNameToIdMap;

       public:
        LogCategories() = default;

        explicit LogCategories(
            const std::set<std::string>& categoryNames,
            const LogLevel&              defaultLogLevel
        );

        LogCategories copy() const;

        LogCategoryId findLogCategory(const std::string& categoryName) const;

        std::vector<LogCategory>   getCategories() const;
        std::optional<LogCategory> getCategory(
            const LogCategoryId& categoryId
        ) const;
        std::optional<LogCategory> getCategory(
            const std::string& categoryName
        ) const;

        std::vector<LogCategoryId> getChildrenOf(
            const LogCategoryId& parentId
        ) const;

        std::vector<LogCategoryId> getAllDescendantsOf(
            const LogCategoryId& categoryId
        ) const;

        void setLogLevel(
            const std::string& categoryName,
            const LogLevel&    logLevel
        );

        //
        // private helper methods starting here
        //

       private:
        [[nodiscard]] LogCategoryId _addLogCategory(
            const std::string& fullName,
            const LogLevel&    logLevel
        );

        [[nodiscard]] LogCategoryId _addLogCategory(
            const LogCategoryId& parentCategoryId,
            const std::string&   segment,
            const std::string&   fullName,
            const LogLevel&      logLevel
        );

        [[nodiscard]] LogCategoryId _getOrCreateLogCategory(
            const LogCategoryId& parentCategoryId,
            const std::string&   segment,
            const std::string&   fullName,
            const LogLevel&      logLevel
        );

        static void _appendSegmentToFullName(
            std::string&       parentFullName,
            const std::string& segment
        );

        template <typename Fn>
        static void _forEachSegment(const std::string& fullName, Fn func);
    };

}   // namespace logging

#ifndef __LOGGING__INCLUDE__LOGGING__LOG_CATEGORIES_TPP__
#include "log_categories.tpp"
#endif   // __LOGGING__INCLUDE__LOGGING__LOG_CATEGORIES_TPP__

#endif   // __LOGGING__INCLUDE__LOGGING__LOG_CATEGORIES_HPP__